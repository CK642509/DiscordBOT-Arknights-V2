from typing import Tuple, Type

from pydantic import BaseModel

from pydantic_settings import (
    BaseSettings,
    PydanticBaseSettingsSource,
    SettingsConfigDict,
    TomlConfigSettingsSource,
)


class DiscodBotSettings(BaseModel):
    token: str


class DiscordSettings(BaseModel):
    bot: DiscodBotSettings
    guild_id: int
    info_channel_id: int
    clue_channel_id: int


class User(BaseModel):
    id: str
    name: str


class Configs(BaseSettings):
    discord: DiscordSettings
    users: list[User]
    model_config = SettingsConfigDict(toml_file="config.toml")

    @classmethod
    def settings_customise_sources(
        cls,
        settings_cls: Type[BaseSettings],
        init_settings: PydanticBaseSettingsSource,
        env_settings: PydanticBaseSettingsSource,
        dotenv_settings: PydanticBaseSettingsSource,
        file_secret_settings: PydanticBaseSettingsSource,
    ) -> Tuple[PydanticBaseSettingsSource, ...]:
        return (TomlConfigSettingsSource(settings_cls),)
