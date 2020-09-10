#include "MyApp.h"
#include "MainFrame.h"
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {

  auto mf = new MainFrame();
  mf->Show();

  return true;
}
