import logging
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

    @commands.Cog.listener()
    async def on_ready(self):
        logger.info("Exchange Cog is ready.")

    @commands.hybrid_command()
    async def result(self, ctx: commands.Context):
        """查看最佳做法"""
        result = ClueProcessor.get_result()
        info_channel = self.bot.get_channel(self.info_channel_id)
        await info_channel.send(result)

    @commands.hybrid_command()
    async def exchange(self, ctx: commands.Context):
        """開始計算最佳做法"""
        logger.info("Exchange command received.")
        start_time = time()
        ClueProcessor.exchange()
        end_time = time()
        elapsed_time = end_time - start_time
        logger.info(f"Exchange command completed. Time elapsed: {elapsed_time:.2f} seconds.")
        


async def setup(bot: commands.Bot):
    await bot.add_cog(ExchangeCog(bot))
