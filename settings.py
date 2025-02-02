from typing import Optional

from pydantic_settings import BaseSettings


class Settings(BaseSettings):
    token: str
    guild_id: Optional[int] = None
    info_channel_id: int
    clue_channel_id: int

settings = Settings()
