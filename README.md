Simple launcher to allow easy modloading and add some simple functionality for mods

Built for Blackwake: https://store.steampowered.com/app/420290/Blackwake/

Simple UI is built using wxWidgets

C# modloader based off da_google's c# modloader

It works, but might require some adjustment from the perspective of the mod with regard to accessing the game's Resources due to the modloader now running on launch, rather than after you enter the menu and all is loaded. It's also worth noting sometimes the game crashes, I have no idea why it does this and altering when it's injected has no effect on the crash chance 🤷‍♀️
