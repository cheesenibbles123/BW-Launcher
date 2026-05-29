Simple hobby made launcher to allow easy modloading and add some simple functionality for mods (Currently only works on windows, linux & mac TBD)

Built for Blackwake: https://store.steampowered.com/app/420290/Blackwake/

Simple UI is built using wxWidgets. (Achievements etc was a personal stretch goal so not working currently)

C# modloader based off da_google's c# modloader with slight tweaks to work with the adjusted folder structure used by this project.

It works, but might require some adjustment from the perspective of the mod with regard to accessing the game's Resources due to the modloader now running on launch, rather than after you enter the menu and all is loaded. It's also worth noting sometimes the game crashes, I have no idea why it does this and altering when it's injected has no effect on the crash chance 🤷‍♀️

## Mod Structure

With the way this modloader works, each mod has a "root" folder within the **Launcher/Mods** folder. Within said root folder the following tree is used:

```
- ModFolder
	| - Assets
	| \ - AchievementIcons
	| - dlls
	\ - achievements.json
	  - config.json
```

### Config

The configuration within the **config.json** and the name of the folder are important. Your config.json should contain the following information

```json
{
    "ModName": "This should also match the folder name of the mod",
    "ModAuthor": "Your name here",
    "ModIconPath": "If you have an icon within the assets folder, point there",
    "ModDllName": "The name of your mod's dll"
}
```

If the **ModName** and the folder name do not match up, achievement updating will not work, so be sure to check that.

When setting up for asset path files within the C#, be sure to take a look at the **ModLoader** class, it contains a few functions to get the correct pathing to help you get started

### Achievements

Achievements are currently setup by having the config within the **achievements.json** file, and related imagery within the **Assets/AchievementIcons/** directory. Below is an example showing how a typical achievement is setup.

```json
[
    {
        "ID": "ACHIEVEMENT_ID_",
        "DisplayText": "Achievement Name",
        "type": 0,
        "currentScore": 0,
        "goalScore": 150,
        "completeImageUrl": "image_incomplete_.png",
        "incompleteImageUrl": "image_complete_.bmp"
    }
]
```

When making your mod, make sure to reference the **Modloader.dll** file, you will be able to make use of the **Tools.CommunicationPipes** class to communicate with the launcher. For the purposes of achievements, you should only need to use the **incrementAchievement** function. (Achievement counters can only increase as of current, so bear that in mind)