#ifndef SOUNDFX_H_
#define SOUNDFX_H_

#include <string>

#if defined(_MSC_VER)

#include "irrKlang/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")
#include <iostream>

class SoundFXController
{
  public:

	void playClip(std::string soundFile)
	{
		if (m_engine != nullptr)
			m_engine->play2D(soundFile.c_str(), false);
	}

	void abortClip()
	{
		if (m_engine != nullptr)
			m_engine->stopAllSounds();
	}

	static SoundFXController& getInstance();

  private:
	irrklang::ISoundEngine* m_engine;

	SoundFXController()
	{
		m_engine = irrklang::createIrrKlangDevice();
		if (m_engine == nullptr)
			std::cout << "Cannot create sound engine!  Game will be silent."
					  << std::endl;
	}

	~SoundFXController()
	{
		if (m_engine != nullptr)
			m_engine->drop();
	}

	SoundFXController(const SoundFXController&);
	SoundFXController& operator=(const SoundFXController&);
};

#elif defined(__APPLE__)

#include <cstdlib>
#include <chrono>

class SoundFXController
{
  public:
	void playClip(std::string soundFile)
	{
          // Don't start a clip more than 2 times per second
        static std::chrono::system_clock::time_point lastPlayTime;
        auto now = std::chrono::system_clock::now();
        if ((now - lastPlayTime).count() >= 0.5) // at least 1/2 second ago
        {
            lastPlayTime = now;
            std::system(("/usr/bin/afplay \"" + soundFile + "\" &").c_str());
        }
    }
	
	void abortClip()
	{
		std::string cmd = "killall afplay";
		std::system(cmd.c_str());
	}

	static SoundFXController& getInstance();

};

#else  // forget about sound

class SoundFXController
{
  public:
	void playClip(std::string /* soundFile */) {}
	void abortClip() {}
	static SoundFXController& getInstance();
};

#endif

  // Meyers singleton pattern
inline SoundFXController& SoundFXController::getInstance()
{
	static SoundFXController instance;
	return instance;
}

inline SoundFXController& SoundFX()
{
	return SoundFXController::getInstance();
}

#endif // SOUNDFX_H_
