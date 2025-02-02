from typing import Optional

import discord
from discord.ext import commands

from settings import settings


# https://github.com/Rapptz/discord.py/blob/master/examples/advanced_startup.py
class MyBot(commands.Bot):
    def __init__(
        self,
        *args,
        initial_extensions: list[str],
        guild_id: Optional[int] = None,
        **kwargs,
    ):
        super().__init__(*args, **kwargs)
        self.guild_id = guild_id
        self.initial_extensions = initial_extensions

    async def setup_hook(self) -> None:
        for extension in self.initial_extensions:
            await self.load_extension(extension)

        if self.guild_id:
            guild = discord.Object(self.guild_id)
            self.tree.copy_global_to(guild=guild)
            await self.tree.sync(guild=guild)


def main():
    exts = [
        "cogs.tutorial",
    ]
    intents = discord.Intents.default()
    intents.message_content = True

    bot = MyBot(
        initial_extensions=exts,
        guild_id=settings.guild_id,
        command_prefix="",
        intents=intents,
    )

    bot.run(settings.token)


if __name__ == "__main__":
    main()
