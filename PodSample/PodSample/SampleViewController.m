/*
 Copyright 2009-2014 Urban Airship Inc. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.

 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC ``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 EVENT SHALL URBAN AIRSHIP INC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "SampleViewController.h"
#import "UAPush.h"
#import "UAirship.h"
#import "UAPushSettingsViewController.h"
#import "UAPushMoreSettingsViewController.h"
#import "UAirship.h"
#import "UAInbox.h"
#import "UAInboxMessageListController.h"

@implementation SampleViewController

- (UAPushSettingsViewController *)buildAPNSSettingsViewController {
    UAPushSettingsViewController *vc = [[UAPushSettingsViewController alloc] initWithNibName:@"UAPushSettingsView"
                                                                                      bundle:nil];

    vc.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                                                                           target:self
                                                                                         action:@selector(closeSettings)];

    return vc;
}

- (UAPushMoreSettingsViewController *)buildTokenSettingsViewController {
    UAPushMoreSettingsViewController *vc = [[UAPushMoreSettingsViewController alloc] initWithNibName:@"UAPushMoreSettingsView"
                                                                                              bundle:nil];
    vc.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc]
                                              initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                              target:self
                                            action:@selector(closeSettings)];

    return vc;
}

/*
 Builds a new instance of the message list controller, configuring buttons and closeBlock implementations.
 */
- (UAInboxMessageListController *)buildMessageListController {
    UAInboxMessageListController *mlc = [[UAInboxMessageListController alloc] initWithNibName:@"UAInboxMessageListController" bundle:nil];
    mlc.title = @"Inbox";

    mlc.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                                                                         target:self
                                                                                         action:@selector(inboxDone:)];

    return mlc;
}

- (IBAction)buttonPressed:(id)sender {
    UIViewController *root;
    if (sender == self.settingsButton) {
        root = [self buildAPNSSettingsViewController];
    } else if (sender == self.tokenButton) {
        root = [self buildTokenSettingsViewController];
    }

    [self presentController:root];
}

- (void)closeSettings {
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)presentController:(UIViewController *)root {
    UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:root];

    [self presentViewController:nav animated:YES completion:nil];
}

#pragma Inbox

- (void)viewDidLoad {
    [super viewDidLoad];

    self.version.text = [NSString stringWithFormat:@"Version: %@", [UAirshipVersion get]];

    self.navigationItem.rightBarButtonItem  = [[UIBarButtonItem alloc] initWithTitle:@"Inbox"
                                                                               style:UIBarButtonItemStylePlain
                                                                              target:self action:@selector(showInbox)];

    [UAirship inbox].delegate = self;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void)inboxDone:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark UAInboxDelegate methods

- (void)showInbox {
    [self presentController:[self buildMessageListController]];
}

@end
