import asyncio
import logging
from discord import app_commands, Interaction
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

        logger.info("Exchange command received.")
        self.is_calculating = True
        await interaction.response.defer()
        # await interaction.followup.send("開始計算！")
        start_time = time()

        async with interaction.channel.typing():
            loop = asyncio.get_running_loop()
            await loop.run_in_executor(None, ClueProcessor.exchange)

        end_time = time()
        elapsed_time = end_time - start_time
        logger.info(
            f"Exchange command completed. Time elapsed: {elapsed_time:.2f} seconds."
        )
        self.is_calculating = False
        await interaction.followup.send("計算完成！")


async def setup(bot: commands.Bot):
    await bot.add_cog(ExchangeCog(bot))
