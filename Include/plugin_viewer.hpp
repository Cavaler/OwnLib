#ifndef __PLUGIN_VIEWER_HPP__
#define __PLUGIN_VIEWER_HPP__

enum VIEWERR_EVENTS {
  VE_READ,
  VE_CLOSE
};

typedef int (WINAPI *FARAPIVIEWERCONTROL)(
  int Command,
  void *Param
);

enum VIEWER_CONTROL_COMMANDS {
  VCTL_GETINFO,
  VCTL_QUIT,
  VCTL_REDRAW,
  VCTL_SETKEYBAR,
  VCTL_SETPOSITION,
  VCTL_SELECT,
};

enum VIEWER_OPTIONS {
  VOPT_SAVEFILEPOSITION=1,
  VOPT_AUTODETECTTABLE=2,
};

typedef union {
  __int64 i64;
  struct {
    DWORD LowPart;
    LONG  HighPart;
  } Part;
} FARINT64;

struct ViewerSelect
{
  FARINT64 BlockStartPos;
  int      BlockLen;
};

enum VIEWER_SETPOS_FLAGS {
  VSP_NOREDRAW    = 0x0001,
  VSP_PERCENT     = 0x0002,
  VSP_RELATIVE    = 0x0004,
  VSP_NORETNEWPOS = 0x0008,
};

struct ViewerSetPosition
{
  DWORD Flags;
  FARINT64 StartPos;
  int   LeftPos;
};

struct ViewerMode{
  int UseDecodeTable;
  int TableNum;
  int AnsiMode;
  int Unicode;
  int Wrap;
  int TypeWrap;
  int Hex;
  DWORD Reserved[4];
};

struct ViewerInfo
{
  int    StructSize;
  int    ViewerID;
  const char *FileName;
  FARINT64 FileSize;
  FARINT64 FilePos;
  int    WindowSizeX;
  int    WindowSizeY;
  DWORD  Options;
  int    TabSize;
  struct ViewerMode CurMode;
  int    LeftPos;
  DWORD  Reserved3;
};

#ifdef __cplusplus
extern "C"{
#endif
int    WINAPI _export ProcessViewerEvent(int Event,void *Param);
#ifdef __cplusplus
};
#endif

#endif /* __PLUGIN_VIEWER_HPP__ */
