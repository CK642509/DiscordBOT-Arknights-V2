import logging
from discord.ext import commands

from settings import settings
from utils.crud_clues import ClueProcessor

logger = logging.getLogger("discord")


class ExchangeCog(commands.Cog):
    def __init__(self, bot: commands.Bot):
        self.bot = bot

    @commands.Cog.listener()
    async def on_ready(self):
        logger.info("Exchange Cog is ready.")

    @commands.hybrid_command()
    async def result(self, ctx: commands.Context):
        """查看最佳做法"""
        result = ClueProcessor.get_result()
        info_channel = self.bot.get_channel(settings.info_channel_id)
        await info_channel.send(result)

    @commands.hybrid_command()
    async def exchange(self, ctx: commands.Context):
        """開始計算最佳做法"""
        pass


async def setup(bot: commands.Bot):
    await bot.add_cog(ExchangeCog(bot))
