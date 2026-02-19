import discord
import logging
from discord.ext import commands

logger = logging.getLogger("discord")


class TutorialCog(commands.Cog):
    def __init__(self, bot: commands.Bot):
        self.bot = bot

    @commands.Cog.listener()
    async def on_ready(self):
        logger.info("Tutorial Cog is ready.")

    @commands.hybrid_command()
    async def tutorial(self, ctx: commands.Context):
        embed = discord.Embed(
            title="使用教學",
            url="https://ithelp.ithome.com.tw/users/20162280/ironman/7781",
        )
        embed.add_field(name="/tutorial", value="查看使用教學", inline=False)
        embed.add_field(name="/exchange", value="開始計算最佳做法", inline=False)
        embed.add_field(name="/result", value="查看最佳做法", inline=False)
        embed.add_field(name="/update", value="根據歷史訊息更新線索紀錄", inline=False)
        embed.add_field(name="/set_clue", value="幫指定玩家更新線索", inline=False)
        await ctx.send(embed=embed)


async def setup(bot: commands.Bot):
    await bot.add_cog(TutorialCog(bot))
