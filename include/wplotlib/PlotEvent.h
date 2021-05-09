#pragma once
#include <wx/event.h>

class MouseHoverEvent;
wxDECLARE_EVENT(PLOT_MOUSE_HOVER_EVENT, MouseHoverEvent);

class MouseHoverEvent : public wxCommandEvent {
public:
  MouseHoverEvent(const wxRealPoint &data,
                  wxEventType commandType = PLOT_MOUSE_HOVER_EVENT, int id = 0)
      : wxCommandEvent(commandType, id), data(data) {}

  wxEvent *Clone() const { return new MouseHoverEvent(*this); }

  MouseHoverEvent(const MouseHoverEvent &event) : wxCommandEvent(event) {
    this->data = event.data;
  }

  wxRealPoint data;
};