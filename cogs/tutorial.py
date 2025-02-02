from discord.ext import commands
import discord


class TutorialCog(commands.Cog):
    def __init__(self, bot: commands.Bot):
        self.bot = bot

    @commands.hybrid_command()
    async def tutorial(self, ctx: commands.Context):
        embed = discord.Embed(
            title="使用教學",
            url="https://ithelp.ithome.com.tw/users/20162280/ironman/7781",
        )
        embed.add_field(name="tutorial", value="查看使用教學", inline=False)
        embed.add_field(name="exchange", value="開始計算最佳做法", inline=False)
        await ctx.send(embed=embed)


async def setup(bot: commands.Bot):
    await bot.add_cog(TutorialCog(bot))
