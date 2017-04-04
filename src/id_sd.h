//
//      ID Engine
//      ID_SD.h - Sound Manager Header
//      Version for Wolfenstein
//      By Jason Blochowiak
//

#ifndef __ID_SD__
#define __ID_SD__

#define alOut(n,b) YM3812Write(0, n, b)

/*
====================================
= {'-'}
= 
= Changing TickBase causes the game speed to change. If you want "n" times normal gamespeed, set this to 70 * n. 
= Note that several other parts of code need to also be changed. 
=
= id_sd.h:
=   line 142: from      ((SDL_GetTicks()*7)/100)                                        to      ((SDL_GetTicks()*(7*n))/100)
=   line 146: from      SDL_Delay(wolfticks * 100 / 7);                                 to      SDL_Delay(wolfticks * 100 / 7*n); 
=
= wl_draw.cpp:
=   line 1291: from     tics = (curtime * 7) / 100 - lasttimecount;                     to      tics = (curtime * 7*n) / 100 - lasttimecount;
=   line 1295: from     SDL_Delay(((lasttimecount + 1) * 100) / 7 - curtime);           to      SDL_Delay(((lasttimecount + 1) * 100) / 7*n - curtime);
=
= wl_play.cpp:
=   line 417: from      int32_t timediff = (lasttimecount * 100) / 7 - curtime;         to      int32_t timediff = (lasttimecount * 100) / 7*n - curtime;
=   line 422: from      lasttimecount = (curtime * 7) / 100;                            to      lasttimecount = (curtime * 7*n) / 100;
====================================
*/
#define TickBase        70      // 70Hz per tick - used as a base for timer 0

typedef enum
{
    sdm_Off,
    sdm_PC,sdm_AdLib,
} SDMode;

typedef enum
{
    smm_Off,smm_AdLib
} SMMode;

typedef enum
{
    sds_Off,sds_PC,sds_SoundBlaster
} SDSMode;

typedef struct
{
    longword        length;
    word            priority;
} SoundCommon;

#define ORIG_SOUNDCOMMON_SIZE 6

//      PC Sound stuff
#define pcTimer         0x42
#define pcTAccess       0x43
#define pcSpeaker       0x61

#define pcSpkBits       3

typedef struct
{
    SoundCommon     common;
    byte            data[1];
} PCSound;

//      Register addresses
// Operator stuff
#define alChar          0x20
#define alScale         0x40
#define alAttack        0x60
#define alSus           0x80
#define alWave          0xe0
// Channel stuff
#define alFreqL         0xa0
#define alFreqH         0xb0
#define alFeedCon       0xc0
// Global stuff
#define alEffects       0xbd

typedef struct
{
    byte    mChar,cChar,
            mScale,cScale,
            mAttack,cAttack,
            mSus,cSus,
            mWave,cWave,
            nConn,

            // These are only for Muse - these bytes are really unused
            voice,
            mode;
    byte    unused[3];
} Instrument;

#define ORIG_INSTRUMENT_SIZE 16

typedef struct
{
    SoundCommon     common;
    Instrument      inst;
    byte            block;
    byte            data[1];
} AdLibSound;

#define ORIG_ADLIBSOUND_SIZE (ORIG_SOUNDCOMMON_SIZE + ORIG_INSTRUMENT_SIZE + 2)

//
//      Sequencing stuff
//
#define sqMaxTracks     10

typedef struct
{
    word    length;
    word    values[1];
} MusicGroup;

typedef struct
{
    int valid;
    fixed globalsoundx, globalsoundy;
} globalsoundpos;

extern globalsoundpos channelSoundPos[];

// Global variables
extern  boolean         AdLibPresent,
                        SoundBlasterPresent,
                        SoundPositioned;
extern  SDMode          SoundMode;
extern  SDSMode         DigiMode;
extern  SMMode          MusicMode;
extern  int             DigiMap[];
extern  int             DigiChannel[];

#define GetTimeCount()  ((SDL_GetTicks()*7)/100)   //350

inline void Delay(int wolfticks)
{
    if(wolfticks>0) SDL_Delay(wolfticks * 100 / 7);    //350
}

// Function prototypes
extern  void    SD_Startup(void),
                SD_Shutdown(void);

extern  int     SD_GetChannelForDigi(int which);
extern  void    SD_PositionSound(int leftvol,int rightvol);
extern  boolean SD_PlaySound(soundnames sound);
extern  void    SD_SetPosition(int channel, int leftvol,int rightvol);
extern  void    SD_StopSound(void),
                SD_WaitSoundDone(void);

extern  void    SD_StartMusic(int chunk);
extern  void    SD_ContinueMusic(int chunk, int startoffs);
extern  void    SD_MusicOn(void),
                SD_FadeOutMusic(void);
extern  int     SD_MusicOff(void);

extern  boolean SD_MusicPlaying(void);
extern  boolean SD_SetSoundMode(SDMode mode);
extern  boolean SD_SetMusicMode(SMMode mode);
extern  word    SD_SoundPlaying(void);

extern  void    SD_SetDigiDevice(SDSMode);
extern  void	SD_PrepareSound(int which);
extern  int     SD_PlayDigitized(word which,int leftpos,int rightpos);
extern  void    SD_StopDigitized(void);

#endif
