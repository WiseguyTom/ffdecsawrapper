
#include <stdlib.h>
#include <stdio.h>

#define TESTER
#include "crypto.h"
#include "data.h"
#include "system-common.h"
#include "compat.h"

#include "systems/viaccess/viaccess.h"
#include "systems/viaccess/viaccess.c"

// ----------------------------------------------------------------

class cTransponderTime;

class cSatTimeHook : public cLogHook {
private:
public:
  cSatTimeHook(cTransponderTime *Ttime);
  ~cSatTimeHook();
  virtual void Process(int pid, unsigned char *data);
  };

cSatTimeHook::cSatTimeHook(cTransponderTime *Ttime)
:cLogHook(HOOK_SATTIME,"sattime")
{}

cSatTimeHook::~cSatTimeHook()
{}

void cSatTimeHook::Process(int pid, unsigned char *data)
{}

// ----------------------------------------------------------------

class cTpsAuHook : public cLogHook {
public:
  cTpsAuHook(void);
  virtual ~cTpsAuHook();
  virtual void Process(int pid, unsigned char *data);
  void DummyProcess(unsigned char *data, int size);
  };

#include "systems/viaccess/tps.c"
#include "systems/viaccess/st20.c"

cTpsAuHook::cTpsAuHook(void)
:cLogHook(HOOK_TPSAU,"tpsau")
{}

cTpsAuHook::~cTpsAuHook()
{}

void cTpsAuHook::Process(int pid, unsigned char *data)
{
}

void cTpsAuHook::DummyProcess(unsigned char *data, int size)
{
  cOpenTVModule mod(2,data,size);
  tpskeys.ProcessAu(&mod);
}

// ----------------------------------------------------------------

int main(int argc, char *argv[])
{
  if(argc<3) {
    printf("usage: %s <plugin-dir> <decomp-bin>\n",argv[0]);
    return 1;
    }

  InitAll(argv[1]);
  LogAll();
  FILE *f=fopen(argv[2],"r");
  if(f) {
    fseek(f,0,SEEK_END);
    int size=ftell(f);
    fseek(f,0,SEEK_SET);
    unsigned char *data=(unsigned char *)malloc(size);
    fread(data,1,size,f);
    fclose(f);
    printf("read %d bytes from %s\n",size,argv[2]);

    cTpsAuHook hook;
    hook.DummyProcess(data,size);
    }
  return 0;
}
