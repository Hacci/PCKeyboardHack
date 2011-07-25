// -*- indent-tabs-mode: nil; -*-

#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "base.hpp"
#include "bridge.h"

// http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptIOKit/hello_iokit.html#//apple_ref/doc/uid/20002366-CIHECHHE
class org_pqrs_driver_PCKeyboardHack : public IOService
{
  OSDeclareDefaultStructors(org_pqrs_driver_PCKeyboardHack);

public:
  virtual bool init(OSDictionary* dictionary = 0);
  virtual void free(void);
  virtual IOService* probe(IOService* provider, SInt32* score);
  virtual bool start(IOService* provider);
  virtual void stop(IOService* provider);

  static void setConfiguration(const BridgeUserClientStruct& newval);
  static void unsetConfiguration(void);

private:
  // see IOHIDUsageTables.h or http://www2d.biglobe.ne.jp/~msyk/keyboard/layout/usbkeycode.html
  class KeyMapIndex {
  public:
    enum Value {
      NONE     = 0, // NONE must be a unique value in this enum.
      CAPSLOCK = 0x39,
      JIS_KANA = 0x88,
      JIS_NFER = 0x8b,
      JIS_XFER = 0x8a,

      //Haci
      COLON    = 0x34,		//08.6.10 KE
      OPTION_L = 0xe2,		//08.6.10
      OPTION_R = 0xe6,		//08.6.10
      ENTER    = 0x58,		//08.7.14 10key
      CONTROL_L = 0xe0,		//2011.07.25
      CONTROL_R = 0xe4,		//2011.07.25

    };
    static Value bridgeKeyindexToValue(int bridgeKeyIndex) {
      switch (bridgeKeyIndex) {
        case BRIDGE_KEY_INDEX_CAPSLOCK: return CAPSLOCK;
        case BRIDGE_KEY_INDEX_JIS_KANA: return JIS_KANA;
        case BRIDGE_KEY_INDEX_JIS_NFER: return JIS_NFER;
        case BRIDGE_KEY_INDEX_JIS_XFER: return JIS_XFER;
        case BRIDGE_KEY_INDEX_COLON: return COLON;         //2011.07.25 Haci
        case BRIDGE_KEY_INDEX_OPTION_L: return OPTION_L;   //2011.07.25 Haci
        case BRIDGE_KEY_INDEX_OPTION_R: return OPTION_R;   //2011.07.25 Haci
        case BRIDGE_KEY_INDEX_ENTER: return ENTER;         //2011.07.25 Haci
        case BRIDGE_KEY_INDEX_CONTROL_L: return CONTROL_L; //2011.07.25 Haci
        case BRIDGE_KEY_INDEX_CONTROL_R: return CONTROL_R; //2011.07.25 Haci
      }
      return NONE;
    }
  };
  enum {
    MAXNUM_KEYBOARD = 16,
  };

  // ------------------------------------------------------------
  class HookedKeyboard {
  public:
    HookedKeyboard(void) : kbd_(NULL) {}

    void initialize(IOHIKeyboard* p);
    void terminate(void);
    void refresh(void);
    IOHIKeyboard* get(void) { return kbd_; }

  private:
    IOHIKeyboard* kbd_;
    unsigned int originalKeyCode_[BRIDGE_KEY_INDEX__END__];
  };

  // ------------------------------------------------------------
  static HookedKeyboard* new_hookedKeyboard(void);
  static HookedKeyboard* search_hookedKeyboard(const IOHIKeyboard* kbd);

  static bool IOHIKeyboard_gIOMatchedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier);
  static bool IOHIKeyboard_gIOTerminatedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier);

  static bool isTargetDevice(IOHIKeyboard* kbd);

  static bool customizeKeyMap(IOHIKeyboard* kbd);
  static bool restoreKeyMap(IOHIKeyboard* kbd);

  static HookedKeyboard hookedKeyboard_[MAXNUM_KEYBOARD];
  static BridgeUserClientStruct configuration_;

  IONotifier* notifier_hookKeyboard_;
  IONotifier* notifier_unhookKeyboard_;
};

#endif
