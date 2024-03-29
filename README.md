# otodecks-juce
Audio mixer application (DJ)

## About the Project
This is an Audio mixer application that can playback audio files and apply filters to modify playback. It has two audio player decks to control basic functions such as playing/stopping audio playback, filters that modify the audio, and a library to load, delete or save audio tracks.

* The library is able to insert audio files, load into the DeckGUIs, and save the inserted audio files such that the user can load the very same files upon restarting the app. 
* The audio files data will be displayed in a tabular format, and there is a search bar allowing users to search for their desired track within the library.
* The 2 audio filters alters the audio source differently. One of the audio filter (IIRFilter) alters the audio source from respective audio players, whereas the other filter (Reverb Filter) alters the mixer source that contains both audio sources from the players. Users are able to control the parameters/variables pertaining to the filters using the sliders.

## Interface
![image](https://user-images.githubusercontent.com/58553029/191198878-fe2606e7-48e4-4c86-a665-21e1126b0856.png)
![image](https://user-images.githubusercontent.com/58553029/191199004-a4c98c07-7c00-40d9-b887-f927e662638b.png)
