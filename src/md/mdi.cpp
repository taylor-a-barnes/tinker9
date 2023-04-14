#include "md/mdi.h"
#include "md/integrator.h"
#include "ff/energy.h"
#include "md/lflpiston.h"
#include "md/misc.h"
#include "md/pq.h"
#include "md/rattle.h"
#include "tool/error.h"
#include "tool/ioprint.h"
#include <tinker/detail/inform.hh>
#include <tinker/detail/mdstuf.hh>
#include <tinker/detail/units.hh>
#include "mdi.h"

namespace tinker {

std::FILE* MDIEngine::outputFile;

void MDIEngine::initialize(std::FILE* o)
{
   outputFile = o;

   // the call to MDI_Init and all other initialization should go here
}

void MDIEngine::run_mdi()
{
   // the call to MDI_Receive_command should go here
}

}
