// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <PreferencePanes/PreferencePanes.h>

@interface PCKeyboardHackPref : NSPreferencePane
{
  IBOutlet id _versionText;
}

- (void) mainViewDidLoad;

@end
