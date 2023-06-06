#include "md/mdiengine.h"
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
#include <string.h>
#include "mdi.h"

namespace tinker {

std::FILE* MDIEngine::outputFile;
MDI_Comm MDIEngine::mdi_comm;
bool MDIEngine::exit_flag = false;
int MDIEngine::target_node = 0;
double MDIEngine::bohrA_conv = 0.529177249;

void MDIEngine::initialize(std::FILE* o)
{
   outputFile = o;

   int ret;
  int is_initialized = 0;
  ret = MDI_Initialized(&is_initialized);
  if ( is_initialized ) {
    mdi_commands();
    ret = MDI_Accept_communicator(&mdi_comm);
    if ( ret ) {
      TINKER_THROW(format("MDI  --  Error in MDI_Accept_communicator\n"));
    }
    
  }
   // the call to MDI_Init and all other initialization should go here
}

void MDIEngine::mdi_commands()
{
  MDI_Register_node("@DEFAULT");
  MDI_Register_command("@DEFAULT", "<@");
  MDI_Register_command("@DEFAULT", "<NATOMS");
  MDI_Register_command("@DEFAULT", "@INIT_MD");

  MDI_Register_node("@INIT_MD");
  MDI_Register_command("@INIT_MD", "<@");
  MDI_Register_command("@INIT_MD", "@");
  MDI_Register_command("@INIT_MD", "@COORDS");
  MDI_Register_command("@INIT_MD", "@FORCES");
  MDI_Register_command("@INIT_MD", "<NATOMS");

  MDI_Register_node("@COORDS");
  MDI_Register_command("@COORDS", "<@");
  MDI_Register_command("@COORDS", "@");
  MDI_Register_command("@COORDS", "@COORDS");
  MDI_Register_command("@COORDS", "@FORCES");
  MDI_Register_command("@COORDS", "<NATOMS");

  MDI_Register_node("@FORCES");
  MDI_Register_command("@FORCES", "<@");
  MDI_Register_command("@FORCES", "@");
  MDI_Register_command("@FORCES", "@COORDS");
  MDI_Register_command("@FORCES", "@FORCES");
  MDI_Register_command("@FORCES", "<NATOMS");
}

void MDIEngine::run_mdi(const char* node)
{
  int ret;
  int is_initialized = 0;
  ret = MDI_Initialized(&is_initialized);
  if ( ret ) {
    TINKER_THROW(format("MDI  --  Error in MDI_Initialized\n"));
  }
  if (not is_initialized) { return; }


  if (target_node)  {
    if ((target_node == 1) && ( strcmp(node, "@COORDS") == 0)) return;
    if ((target_node == 2) && ( strcmp(node, "@FORCES") == 0)) return;
  }


  /* MDI command from the driver */
  char* command = new char[MDI_COMMAND_LENGTH];
 
  /* Main MDI loop */
  while (not exit_flag) {
    /* Receive a command from the driver */
    ret = MDI_Recv_command(command, mdi_comm);
    if ( ret ) {
       TINKER_THROW(format("MDI  --  Error in MDI_Recv_command\n"));
    }

    /* Respond to the received command */
    if ( strcmp(command, "EXIT") == 0 ) {
      exit_flag = true;
    }
    else if ( strcmp(command, "@") == 0 ) {
      break;
    }
    else if ( strcmp(command, "<@") == 0 ) {
      ret = MDI_Send(node, MDI_NAME_LENGTH, MDI_CHAR, mdi_comm);
    }
    else if ( strcmp(command, "<NATOMS") == 0 ) {
      ret = MDI_Send(&n, 1, MDI_INT, mdi_comm);
    }
    else if ( strcmp(command, "<COORDS") == 0 ) {
      double coords[n*3];
      for (int i=0; i<n ; i++)  {
        coords[3*i]= x[i] / bohrA_conv;
        coords[3*i + 1]= y[i] / bohrA_conv;
        coords[3*i + 2]= z[i] / bohrA_conv;
      }
      // units should be in Bohr not Angstroms, 1.22something
      ret = MDI_Send(coords, n * 3, MDI_DOUBLE, mdi_comm);
    }
    else if ( strcmp(command, ">COORDS") == 0 ) {
      double recv_buffer[n*3];
      ret = MDI_Recv(recv_buffer, n * 3, MDI_DOUBLE, mdi_comm);
      for (int i=0; i<n ; i++)  {
        mdiprint("%f\n",x[i]);
        x[i]=recv_buffer[3*i] * bohrA_conv;
        y[i]=recv_buffer[3*i + 1] * bohrA_conv;
        z[i]=recv_buffer[3*i + 2] * bohrA_conv;
      }
      // units converted back into angstroms frpm Bohr
    }
    else if ( strcmp(command, "<FORCES") == 0 ) {
      double forces[n*3];

      
      for (int i=0; i<n ; i++)  {
        forces[3*i]= - gx[i];
        forces[3*i + 1]= - gy[i];
        forces[3*i + 2]= - gz[i];
      }
      // units should be in atomic units. (something/angstrom)
      ret = MDI_Send(forces, n * 3, MDI_DOUBLE, mdi_comm);
    }
    else if ( strcmp(command, "<MASSES") == 0 ) {
      ret = MDI_Send(mass, n, MDI_DOUBLE, mdi_comm);
    }
    else if ( strcmp(command, ">MASSES") == 0 ) {
      ret = MDI_Recv(mass, n, MDI_DOUBLE, mdi_comm);
    }
    else if ( strcmp(command, "<PE") == 0 ) {
      ret = MDI_Recv(&esum, n, MDI_DOUBLE, mdi_comm);
    }
    else if ( strcmp(command, "<KE") == 0 ) {
      ret = MDI_Recv(&eksum, n, MDI_DOUBLE, mdi_comm);
    }
    else if ( strcmp(command, "<ENERGY") == 0 ) {
      double energy = esum + eksum;
      ret = MDI_Recv(&energy, 1, MDI_DOUBLE, mdi_comm);
    }
    else if ( strcmp(command, "@INIT_MD") == 0 ) {
      break;
    }
    else if ( strcmp(command, "@COORDS") == 0 ) {
      target_node = 1;
      break;
    }
    else if ( strcmp(command, "@FORCES") == 0 ) {
      target_node = 2;
      break;
    }
    
    else {
      TINKER_THROW(format("MDI  --  Received unsupported command: %s\n",
         command));
    }
  }

  // Free any memory allocations
  delete [] command;

}

}
