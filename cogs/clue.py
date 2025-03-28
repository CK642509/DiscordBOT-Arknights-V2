import discord
import logging
from discord import app_commands
from discord.app_commands import Choice
from discord.ext import commands

from config import Configs
from utils.crud_clues import ClueProcessor


CHANNEL_HISTORY_LIMIT = 20

logger = logging.getLogger("discord")


class ClueCog(commands.Cog):
    configs = Configs()
    usernames = [user.name for user in configs.users]
    clue_channel_id = configs.discord.clue_channel_id
    info_channel_id = configs.discord.info_channel_id

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
        clue_channel = self.bot.get_channel(self.clue_channel_id)
        messages = [
            msg async for msg in clue_channel.history(limit=CHANNEL_HISTORY_LIMIT)
        ]

        latest_messages: dict[int, discord.Message] = {}
        for message in messages:
            message_date = message.created_at
            # only listen to message that is created within 1 day
            if (discord.utils.utcnow() - message_date).days > 1:
                continue

            # only listen to message that is not from the bot
            if message.author == self.bot.user:
                continue

            # only listen to the latest message from each user
            if message.author.id not in latest_messages:
                latest_messages[message.author.id] = message
            elif message_date > latest_messages[message.author.id].created_at:
                latest_messages[message.author.id] = message

        for message in latest_messages.values():
            ClueProcessor.handle_clue_message(message.author.id, message.content)

        # return the updated detail
        detail = ClueProcessor.get_detail()
        info_channel = self.bot.get_channel(self.info_channel_id)
        await info_channel.send(f"```{detail}```")

    # Although using Literal as user's type hint is much more readable, it's not flexible.
    # For example, if we want to get the usernames from the config file, we can't use Literal.
    @app_commands.command(name="更新線索")
    @app_commands.choices(user=[Choice(name=user, value=user) for user in usernames])
    async def set_clue(
        self, interaction: discord.Interaction, user: Choice[str], clue: str
    ):
        """幫指定玩家更新線索"""
        # update clue
        formatted_clue = ClueProcessor.format_clue(clue)
        if not ClueProcessor.validate_clue(formatted_clue):
            await interaction.response.send_message("請輸入有效的線索格式")
            return
        ClueProcessor.update_clue(user.name, formatted_clue)

        # send the updated detail to the info channel
        detail = ClueProcessor.get_detail()
        info_channel = self.bot.get_channel(self.info_channel_id)
        await info_channel.send(f"```{detail}```")

        await interaction.response.send_message(f"{user.name} 的線索已更新為 {clue}")

    @commands.Cog.listener()
    async def on_message(self, message: discord.Message):
        if message.author == self.bot.user:
            return

        # if message.channel.id == self.info_channel_id:
        #     return

        # TODO: validate clue format
        # update clue if the message is from the clue channel
        if message.channel.id == self.clue_channel_id:
            ClueProcessor.handle_clue_message(message.author.id, message.content)

            # send the updated detail to the info channel
            detail = ClueProcessor.get_detail()
            info_channel = self.bot.get_channel(self.info_channel_id)
            await info_channel.send(f"```{detail}```")

    @commands.Cog.listener()
    async def on_message_edit(self, before: discord.Message, after: discord.Message):
        if before.author == self.bot.user:
            return

        # only listen to a clue channel
        if before.channel.id != self.clue_channel_id:
            return

        # only listen to message that is created within 1 day
        if (discord.utils.utcnow() - before.created_at).days > 1:
            return

        # set the clue to the new content
        ClueProcessor.handle_clue_message(before.author.id, after.content)

        # send the updated detail to the info channel
        detail = ClueProcessor.get_detail()
        info_channel = self.bot.get_channel(self.info_channel_id)
        await info_channel.send(f"```{detail}```")

    @commands.Cog.listener()
    async def on_message_delete(self, message: discord.Message):
        if message.author == self.bot.user:
            return

        # only listen to a clue channel
        if message.channel.id != self.clue_channel_id:
            return

        # only listen to message that is created within 1 day
        if (discord.utils.utcnow() - message.created_at).days > 1:
            return

        # reset the clue to "0 0"
        ClueProcessor.handle_clue_message(message.author.id, "0 0")

        # send the updated detail to the info channel
        detail = ClueProcessor.get_detail()
        info_channel = self.bot.get_channel(self.info_channel_id)
        await info_channel.send(f"```{detail}```")


async def setup(bot: commands.Bot):
    await bot.add_cog(ClueCog(bot))
