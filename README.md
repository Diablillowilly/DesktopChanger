# DesktopChanger
Marklin Desktop Changer

MDC will automatically go to <a href="https://www.maerklin.de/de/service/multimedia/hintergrundbilder/hintergrundbilder/">marklin backgrounds webpage</a> and fetch the last 1920*1080 desktop, download it, at set it as your background.
To be executed every time your computer boots

How to use:
1. Download the zip from the releases page - you can download the complete zip which comes with the <a href="https://github.com/Diablillowilly/DesktopChangerConfigurator/">Desktop Changer Configurator</a>, which is an user interface to configure the settings
2. Unzip it whereever you want
3. Create a shortcut to "desktop_changer_marklin.exe"
4. Press Windows Key + R, type "shell:startup", and place there the shortcut
5. Done - the program will be executed every time your computer boots

Notes:
 - By default the program will just keep the background updated, it wont store it, and neither will it download the previous month background
 - The program will not do anything, if in a previous boot, it has already updated the background, it will just open, and close inmediately

Stuff to add:
 - Improve the instructions
 - Refactor the code
 
 
Stuff already done:
 - ~~Posibility to redownload past month wallpaper, which will have the calendar removed~~
 - ~~Use XML parser to look through the HTML (made own parser)~~
 - ~~Separate and organize the code into different classes~~
 - ~~Make it check if it is a new month before doing anything~~
 
 - ~~Add some settings:~~
   - ~~Select wanted resolution~~
   - ~~If you want to store the backgrounds~~
   - ~~Where to store the backgrounds~~
