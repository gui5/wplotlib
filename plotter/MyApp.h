#pragma once
#include <wx/wx.h>
class MyApp : public wxApp {

public:
  bool OnInit() override;
};

wxDECLARE_APP(MyApp);