import discord
import logging
from discord.ext import commands

from settings import settings
from utils.crud_clues import ClueProcessor


logger = logging.getLogger("discord")


class ClueCog(commands.Cog):
    def __init__(self, bot: commands.Bot):
        self.bot = bot

    @commands.Cog.listener()
    async def on_ready(self):
        logger.info("Clue Cog is ready.")

    @commands.hybrid_command()
    async def detail(self, ctx: commands.Context):
        """查看線索紀錄"""
        detail = ClueProcessor.get_detail()
        embed = discord.Embed(
            title="線索紀錄",
            description=f"```{detail}```",
        )
        # embed.add_field(name="紀錄", value=f"```{detail}```", inline=False)

        await ctx.send(embed=embed, ephemeral=True)

    @commands.hybrid_command()
    async def update(self, ctx: commands.Context):
        """根據歷史訊息更新線索紀錄"""
        pass

    @commands.Cog.listener()
    async def on_message(self, message: discord.Message):
        if message.author == self.bot.user:
            return

        # Only listen to a info channel
        if message.channel.id != settings.info_channel_id:
            return

        # TODO: get clue from message and update

    @commands.Cog.listener()
    async def on_message_edit(self, before: discord.Message, after: discord.Message):
        if before.author == self.bot.user:
            return

        # Only listen to a info channel
        if before.channel.id != settings.info_channel_id:
            return
        
        # TODO: update clue

    @commands.Cog.listener()
    async def on_message_delete(self, message: discord.Message):
        if message.author == self.bot.user:
            return

        # Only listen to a info channel
        if message.channel.id != settings.info_channel_id:
            return

        # TODO: reset the clue to "0 0"


async def setup(bot: commands.Bot):
    await bot.add_cog(ClueCog(bot))
