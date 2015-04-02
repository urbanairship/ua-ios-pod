/*
 Copyright 2009-2015 Urban Airship Inc. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

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

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "UAInAppMessageControllerDelegate.h"

@class UAInAppMessage;

/**
 * Controller interface for showing and dismissing in-app
 * messages.
 */
@interface UAInAppMessageController : NSObject<UIGestureRecognizerDelegate>

/**
 * UAInAppMessageController initializer.
 * @param message An instance of UAInAppMessage.
 * @param delegate An object implementing the UAInAppMessageControllerDelegate protocol.
 * @param dismissalBlock A block that will be executed once the message is dismissed.
 * @return An instance of UAInAppMessageController.
 */
+ (instancetype)controllerWithMessage:(UAInAppMessage *)message
                             delegate:(id<UAInAppMessageControllerDelegate>)delegate
                       dismissalBlock:(void(^)(void))dismissalBlock;
/**
 * Show the associated message.
 */
- (void)show;

/**
 * Dismiss the associated message.
 */
- (void)dismiss;

@property(nonatomic, readonly) UAInAppMessage *message;

@end

