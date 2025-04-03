import asyncio
import logging
from discord import app_commands, ButtonStyle, Interaction, ui
from discord.ext import commands
from time import time

from config import Configs
from utils.crud_clues import ClueProcessor

logger = logging.getLogger("discord")


class ExchangeCog(commands.Cog):
    configs = Configs()
    usernames = [user.name for user in configs.users]
    clue_channel_id = configs.discord.clue_channel_id
    info_channel_id = configs.discord.info_channel_id

    def __init__(self, bot: commands.Bot):
        self.bot = bot
        self.is_calculating = False

    @commands.Cog.listener()
    async def on_ready(self):
        logger.info("Exchange Cog is ready.")

    @commands.hybrid_command()
    async def result(self, ctx: commands.Context):
        """查看最佳做法"""
        if self.is_calculating:
            await ctx.send("計算正在進行中，請稍後再試！")
            return

        result = ClueProcessor.get_result()
        info_channel = self.bot.get_channel(self.info_channel_id)
        await info_channel.send(result)

    @app_commands.command()
    async def exchange(self, interaction: Interaction):
        """開始計算最佳做法"""
        if self.is_calculating:
            await interaction.response.send_message("計算正在進行中，請稍後再試！")
            return

        everyone_updated = ClueProcessor.check_update_date()
        if not everyone_updated:
            view = ui.View()
            btn = ui.Button(label="交換線索", style=ButtonStyle.primary)
            btn.callback = self.on_start
            cancel_btn = ui.Button(label="取消")
            cancel_btn.callback = self.on_cancel

            view.add_item(btn)
            view.add_item(cancel_btn)
            await interaction.response.send_message(
                "有人的線索尚未更新，確認要開始交換嗎?", view=view
            )
            return

        await self.start_exchange(interaction, defer=True)

    async def on_cancel(self, interaction: Interaction):
        await interaction.response.edit_message(content="已取消交換", view=None)

    async def on_start(self, interaction: Interaction):
        await interaction.response.edit_message(content="確認開始交換", view=None)
        await self.start_exchange(interaction)

    async def start_exchange(self, interaction: Interaction, defer: bool = False):
        logger.info("Exchange command received.")
        self.is_calculating = True
        if defer:
            await interaction.response.defer()
        initial_message = await interaction.followup.send("開始計算！", wait=True)
        start_time = time()

        async with interaction.channel.typing():
            loop = asyncio.get_running_loop()
            generator = await loop.run_in_executor(None, ClueProcessor.exchange)

            # Process the yielded strings
            for output_line in generator:
                if output_line.startswith("已計算"):
                    await interaction.followup.edit_message(
                        initial_message.id, content=output_line
                    )

        end_time = time()
        elapsed_time = end_time - start_time
        logger.info(
            f"Exchange command completed. Time elapsed: {elapsed_time:.2f} seconds."
        )
        self.is_calculating = False
        await interaction.followup.edit_message(
            initial_message.id, content="計算完成！"
        )


async def setup(bot: commands.Bot):
    await bot.add_cog(ExchangeCog(bot))
