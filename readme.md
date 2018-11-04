# Background
Hi there. This is a small partially functional music player which I worked on shortly after really getting into programming. I had taken a small handful of programming courses during my undergrad but never quite got into it. During an internship I faced the necessity of using Python for generating plots and I really got into it. I sometimes DJ for swing dancing events and wasn't happy with any of the applications available. So I decided on the ambitious undertaking of making my own player.

As you might have noticed, I mentioned Python but this is a C++ codebase. I started in Python using PyQT4, but found the documentation lacking (I almost always ended up reading the C++ docs) and the documentation and support for libraries like audio backends and such made the task very tedious. I decided since I spend so much time on the C++ docs, I would just rewrite it in C++ directly. That is the version you see here.

To warn you now, the task become too much of a time sink and I found that the open source DJ Software [Mixxx](https://www.mixxx.org/) had some features in the works, especially regarding UI redesigns, such that I was happy to start using (and contributing here and there) to it. Thus, this project was left behind to collect dust.

Although I never reach what I would call a version 0.1 for this software, it was a tremendous learning experience. It was my first major programming experience outside of doing calculations of plotting for scientific endeavours. I learned a fair bit about classes and inheritence, GUI development, the C++ language itself, the importance of version control, and I even dipped my toes into design patterns (MVC for the library and the mediator pattern for the application). 

# The Code
There are few other things you might notice if you look at the code. Most of them can be chalked up to "I didn't know better". Like I said, it was a learning experience. Some of those things are 1) I just plainly included the dependencies for VLC and TagLib directly. 2) There are only a handful of commits. I only just started learning about Git before giving up on the project. 3) I've included a copy of the latest build directory. I never tried compiling this on another machine so having a copy of the build may allow you to run the program without compiling. 

Also, if you look at the GUI, there are two independent playdecks. Since I was making this for DJing, I wanted two independent playlists available, one for outputing to the main system, and another for running over a USB soundcard to headphones for previewing. I believe the solution for discovering the devices at the time was Windows specific and it wouldn't work on Linux, but I'm not certain.

Speaking of compiling, you must use QT Creator with the MusicPlayerCPP.pro file so that the environmental variables get set properly. In that file you would also need to change the INCLUDEPATH line, as it is absolute and specific to my machine at present.

# Conclusion
I doubt very many people will ever see this, but despite being an incomplete novice project, it is something that I am proud of and wanted to upload for posterity. Who knows, maybe one person one day might find something useful here.
