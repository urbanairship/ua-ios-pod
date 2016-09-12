/*
 Copyright 2009-2016 Urban Airship Inc. All rights reserved.

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

#import "UAGlobal.h"
#import "UAirship.h"
#import "UANamedUser.h"
#import "UAChannelRegistrar.h"
#import "UANotificationContent.h"
#import "UANotificationResponse.h"

@class UANotificationCategory;

NS_ASSUME_NONNULL_BEGIN

/**
 * NSNotification event when the channel is created. The event
 * will contain the channel ID under `UAChannelCreatedEventChannelKey`
 * and a flag under `UAChannelCreatedEventExistingKey` indicating if the
 * the channel was restored or a new channel was created.
 */
extern NSString *const UAChannelCreatedEvent;

/**
 * Channel ID key for the channel created event.
 */
extern NSString *const UAChannelCreatedEventChannelKey;

/**
 * Channel existing key for the channel created event.
 */
extern NSString *const UAChannelCreatedEventExistingKey;

/**
 * Notification options
 */
typedef NS_OPTIONS(NSUInteger, UANotificationOptions) {
    UANotificationOptionBadge   = (1 << 0),
    UANotificationOptionSound   = (1 << 1),
    UANotificationOptionAlert   = (1 << 2),
    UANotificationOptionCarPlay = (1 << 3)
};

/**
 * Notification option for notification type `none`.
 * Not included in UANotificationOptions enum to maintain parity with UNAuthorizationOptions.
 */
static const UANotificationOptions UANotificationOptionNone =  0;

//---------------------------------------------------------------------------------------
// UARegistrationDelegate
//---------------------------------------------------------------------------------------

/**
 * Implement this protocol and add as a [UAPush registrationDelegate] to receive
 * registration success and failure callbacks.
 *
 */
@protocol UARegistrationDelegate <NSObject>
@optional

/**
 * Called when the device channel registers with Urban Airship. Successful
 * registrations could be disabling push, enabling push, or updating the device
 * registration settings.
 *
 * The device token will only be available once the application successfully
 * registers with APNS.
 *
 * When registration finishes in the background, any async tasks that are triggered
 * from this call should request a background task.
 * @param channelID The channel ID string.
 * @param deviceToken The device token string.
 */
- (void)registrationSucceededForChannelID:(NSString *)channelID deviceToken:(NSString *)deviceToken;

/**
 * Called when the device channel failed to register with Urban Airship.
 *
 * When registration finishes in the background, any async tasks that are triggered
 * from this call should request a background task.
 */
- (void)registrationFailed;

@end


//---------------------------------------------------------------------------------------
// UAPushNotificationDelegate Protocol
//---------------------------------------------------------------------------------------

/**
 * Protocol to be implemented by push notification clients. All methods are optional.
 */
@protocol UAPushNotificationDelegate<NSObject>

@optional

/**
 * Called when a notification is received in the foreground.
 *
 * @param notificationContent UANotificationContent object representing the notification info.
 *
 * @param completionHandler the completion handler to execute when notification processing is complete.
 */
-(void)receivedForegroundNotification:(UANotificationContent *)notificationContent completionHandler:(void (^)())completionHandler;

/**
 * Called when a notification is received in the background.
 *
 * @param notificationContent UANotificationContent object representing the notification info.
 *
 * @param completionHandler the completion handler to execute when notification processing is complete.
 */
-(void)receivedBackgroundNotification:(UANotificationContent *)notificationContent completionHandler:(void (^)(UIBackgroundFetchResult))completionHandler;

/**
 * Called when a notification is received in the background or foreground and results in a user interaction.
 * User interactions can include launching the application from the push, or using an interactive control on the notification interface
 * such as a button or text field.
 *
 * @param notificationResponse UANotificationResponse object representing the user's response
 * to the notification and the associated notification contents.
 *
 * @param completionHandler the completion handler to execute when processing the user's response has completed.
 */
-(void)receivedNotificationResponse:(UANotificationResponse *)notificationResponse completionHandler:(void (^)())completionHandler;

/**
 * Called when a notification has arrived in the foreground and is available for display.
 *
 * Note: this method is relevant only for iOS 10 and above.
 *
 * @param notification The notification.
 * @return a UNNotificationPresentationOptions enum value indicating the presentation options for the notification.
 */
- (UNNotificationPresentationOptions)presentationOptionsForNotification:(UNNotification *)notification;

@end


//---------------------------------------------------------------------------------------
// UAPush Class
//---------------------------------------------------------------------------------------

/**
 * This singleton provides an interface to the functionality provided by the Urban Airship iOS Push API.
 */
#pragma clang diagnostic push
@interface UAPush : NSObject


///---------------------------------------------------------------------------------------
/// @name Push Notifications
///---------------------------------------------------------------------------------------

/**
 * Enables/disables background remote notifications on this device through Urban Airship.
 * Defaults to `YES`.
 */
@property (nonatomic, assign) BOOL backgroundPushNotificationsEnabled;

/**
 * Sets the default value for backgroundPushNotificationsEnabled. The default is `YES`.
 * After the backgroundPushNotificationsEnabled value has been directly set, this
 * value has no effect.
 */
@property (nonatomic, assign) BOOL backgroundPushNotificationsEnabledByDefault;

/**
 * Enables/disables user notifications on this device through Urban Airship.
 * Defaults to `NO`. Once set to `YES`, the user will be prompted for remote notifications.
 *
 * On iOS 8+, we recommend that you do not change this value to `NO` and instead direct users to
 * the iOS Settings App. As such, the transition from `YES` to `NO` is disabled by default on iOS 8+.
 * Please see requireSettingsAppToDisableUserNotifications for details.
 */
@property (nonatomic, assign) BOOL userPushNotificationsEnabled;

/**
 * Enables/disables sending the device token during channel registration.
 * Defaults to `YES`. If set to `NO`, the app will not be able to receive push
 * notifications.
 */
@property (nonatomic, assign) BOOL pushTokenRegistrationEnabled;

/**
 * This setting controls the underlying behavior of the SDK when user notifications are disabled.
 * When set to `NO` and user notifications are disabled with the userPushNotificationsEnabled
 * property, this SDK will mark the device as opted-out on the Urban Airship server but the OS-level
 * settings will still show this device as able to receive user notifications.
 *
 * This is a temporary flag to work around an issue in iOS 8 where
 * unregistering user notification types may prevent the device from being able to
 * register with other types without a device restart. It will be removed once
 * the issue is addressed in iOS 8.
 *
 * This setting defaults to `YES` due to the new flag requireSettingsAppToDisableUserNotifications.
 * To enable UA SDK 5 behavior, set this flag and requireSettingsAppToDisableUserNotifications
 * to `NO`.
 *
 */
@property (nonatomic, assign) BOOL allowUnregisteringUserNotificationTypes;

/**
 * This setting controls the behavior of the userPushNotificationsEnabled setting. If set to `YES`, the
 * application will not be allowed to set userPushNotificationsEnabled to `NO`, and instead, the user should
 * be directed to the iOS Settings app via the UIApplicationOpenSettingsURLString URL constant. The iOS
 * Settings app is the preferred method of disabling user notifications as of iOS 8.
 * 
 * The setting defaults to `YES` on iOS 8+. Changing this setting to `NO` could allow notifications with user-visible components
 * (badge, alert, or sound) to be processed by the OS if the notification also has a background `content-available`
 * flag in the `aps` section of the notification.
 *
 * On versions of iOS prior to iOS 8, this flag will always return `NO`. Those iOS versions do not allow linking
 * to the Settings app and are unaffected by the opt-out after opt-in bug.
 *
 * To open the iOS Settings app directly to your application's settings:
 * `[[UIApplication sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]]`
 */
@property (nonatomic, assign) BOOL requireSettingsAppToDisableUserNotifications;


/**
 * Sets the default value for userPushNotificationsEnabled. The default is `NO`.
 * After the userPushNotificationsEnabled value has been directly set, this value
 * has no effect.
 */
@property (nonatomic, assign) BOOL userPushNotificationsEnabledByDefault;

/**
 * The device token for this device, as a hex string.
 */
@property (nonatomic, copy, readonly, nullable) NSString *deviceToken;

/**
 * The channel ID for this device.
 */
@property (nonatomic, copy, readonly, nullable) NSString *channelID;

/**
 * User Notification options this app will request from APNS. Changes to this value
 * will not take effect the next time the app registers with
 * updateRegistration.
 *
 * Defaults to alert, sound and badge.
 */
@property (nonatomic, assign) UANotificationOptions notificationOptions;

/**
 * Custom notification categories. Urban Airship default notification
 * categories will be unaffected by this field.
 *
 * Changes to this value will not take effect until the next time the app registers
 * with updateRegistration.
 */
@property (nonatomic, strong) NSSet<UANotificationCategory *> *customCategories;

/**
 * The combined set of notification categories from `customCategories` set by the app
 * and the Urban Airship provided categories.
 */
@property (nonatomic, readonly) NSSet<UANotificationCategory *> *combinedCategories;

/**
 * Sets authorization required for the default Urban Airship categories. Only applies
 * to background user notification actions.
 *
 * Changes to this value will not take effect until the next time the app registers
 * with updateRegistration.
 */
@property (nonatomic, assign) BOOL requireAuthorizationForDefaultCategories;

/**
 * Set a delegate that implements the UAPushNotificationDelegate protocol.
 */
@property (nonatomic, weak, nullable) id<UAPushNotificationDelegate> pushNotificationDelegate;

/**
 * Set a delegate that implements the UARegistrationDelegate protocol.
 */
@property (nonatomic, weak, nullable) id<UARegistrationDelegate> registrationDelegate;

/**
 * Notification response that launched the application.
 */
@property (nonatomic, readonly, strong, nullable) UANotificationResponse *launchNotificationResponse;

/**
 * The current authorized notification options.
 */
@property (nonatomic, assign, readonly) UANotificationOptions authorizedNotificationOptions;

/**
 * The default presentation options to use for foreground notifications.
 *
 * Note: this property is relevant only for iOS 10 and above.
 */
@property (nonatomic, assign) UNNotificationPresentationOptions defaultPresentationOptions;

///---------------------------------------------------------------------------------------
/// @name Autobadge
///---------------------------------------------------------------------------------------

/**
 * Toggle the Urban Airship auto-badge feature. Defaults to `NO` If enabled, this will update the
 * badge number stored by Urban Airship every time the app is started or foregrounded.
 */
@property (nonatomic, assign, getter=isAutobadgeEnabled) BOOL autobadgeEnabled;

/**
 * Sets the badge number on the device and on the Urban Airship server.
 * 
 * @param badgeNumber The new badge number
 */
- (void)setBadgeNumber:(NSInteger)badgeNumber;

/**
 * Resets the badge to zero (0) on both the device and on Urban Airships servers. This is a
 * convenience method for `setBadgeNumber:0`.
 */
- (void)resetBadge;



///---------------------------------------------------------------------------------------
/// @name Alias
///---------------------------------------------------------------------------------------
 
/** Alias for this device */
@property (nonatomic, copy, nullable) NSString *alias;


///---------------------------------------------------------------------------------------
/// @name Tags
///---------------------------------------------------------------------------------------

/** Tags for this device. */
#if __has_feature(objc_generics)
@property (nonatomic, copy) NSArray<NSString *> *tags;
#else
@property (nonatomic, copy) NSArray *tags;
#endif

/**
 * Allows setting tags from the device. Tags can be set from either the server or the device, but
 * not both (without synchronizing the data), so use this flag to explicitly enable or disable
 * the device-side flags.
 *
 * Set this to `NO` to prevent the device from sending any tag information to the server when using
 * server-side tagging. Defaults to `YES`.
 */
@property (nonatomic, assign, getter=isChannelTagRegistrationEnabled) BOOL channelTagRegistrationEnabled;

/**
 * Adds a tag to the list of tags for the device.
 * To update the server, make all of your changes, then call
 * `updateRegistration` to update the Urban Airship server.
 *
 * @note When updating multiple server-side values (tags, alias, time zone, quiet time), set the
 * values first, then call `updateRegistration`. Batching these calls improves performance.
 *
 * @param tag Tag to be added
 */
- (void)addTag:(NSString *)tag;

/**
 * Adds a group of tags to the current list of device tags. To update the server, make all of your
 * changes, then call `updateRegistration`.
 *
 * @note When updating multiple server-side values (tags, alias, time zone, quiet time), set the
 * values first, then call `updateRegistration`. Batching these calls improves performance.
 *
 * @param tags Array of new tags
 */
#if __has_feature(objc_generics)
- (void)addTags:(NSArray<NSString *> *)tags;
#else
- (void)addTags:(NSArray *)tags;
#endif

/**
 * Removes a tag from the current tag list. To update the server, make all of your changes, then call
 * `updateRegistration`.
 *
 * @note When updating multiple server-side values (tags, alias, time zone, quiet time), set the
 * values first, then call `updateRegistration`. Batching these calls improves performance.
 *
 * @param tag Tag to be removed
 */
- (void)removeTag:(NSString *)tag;

/**
 * Removes a group of tags from a device. To update the server, make all of your changes, then call
 * `updateRegistration`.
 *
 * @note When updating multiple server-side values (tags, alias, time zone, quiet time), set the
 * values first, then call `updateRegistration`. Batching these calls improves performance.
 *
 * @param tags Array of tags to be removed
 */
#if __has_feature(objc_generics)
- (void)removeTags:(NSArray<NSString *> *)tags;
#else
- (void)removeTags:(NSArray *)tags;
#endif

///---------------------------------------------------------------------------------------
/// @name Tag Groups
///---------------------------------------------------------------------------------------

/**
 * Add tags to channel tag groups. To update the server,
 * make all of your changes, then call `updateRegistration`.
 *
 * @param tags Array of tags to add.
 * @param tagGroupID Tag group ID string.
 */
#if __has_feature(objc_generics)
- (void)addTags:(NSArray<NSString *> *)tags group:(NSString *)tagGroupID;
#else
- (void)addTags:(NSArray *)tags group:(NSString *)tagGroupID;
#endif

/**
 * Removes tags from channel tag groups. To update the server,
 * make all of your changes, then call `updateRegistration`.
 *
 * @param tags Array of tags to remove.
 * @param tagGroupID Tag group ID string.
 */
#if __has_feature(objc_generics)
- (void)removeTags:(NSArray<NSString *> *)tags group:(NSString *)tagGroupID;
#else
- (void)removeTags:(NSArray *)tags group:(NSString *)tagGroupID;
#endif

///---------------------------------------------------------------------------------------
/// @name Quiet Time
///---------------------------------------------------------------------------------------

/**
 * Quiet time settings for this device.
 */
@property (nonatomic, copy, readonly, nullable) NSDictionary *quietTime;

/**
 * Time Zone for quiet time. If the time zone is not set, the current
 * local time zone is returned.
 */
@property (nonatomic, strong) NSTimeZone *timeZone;

/**
 * Enables/Disables quiet time
 */
@property (nonatomic, assign, getter=isQuietTimeEnabled) BOOL quietTimeEnabled;

/**
 * Sets the quiet time start and end time.  The start and end time does not change
 * if the time zone changes.  To set the time zone, see 'timeZone'.
 *
 * Update the server after making changes to the quiet time with the
 * `updateRegistration` call. Batching these calls improves API and client performance.
 *
 * @warning This method does not automatically enable quiet time and does not
 * automatically update the server. Please refer to `quietTimeEnabled` and 
 * `updateRegistration` methods for more information.
 *
 * @param startHour Quiet time start hour. Only 0-23 is valid.
 * @param startMinute Quiet time start minute. Only 0-59 is valid.
 * @param endHour Quiet time end hour. Only 0-23 is valid.
 * @param endMinute Quiet time end minute. Only 0-59 is valid.
 */
-(void)setQuietTimeStartHour:(NSUInteger)startHour
                 startMinute:(NSUInteger)startMinute
                     endHour:(NSUInteger)endHour
                   endMinute:(NSUInteger)endMinute;


///---------------------------------------------------------------------------------------
/// @name Channel Registration
///---------------------------------------------------------------------------------------

/**
 * Enables channel creation if channelCreationDelayEnabled was set to `YES` in the config.
 */
- (void)enableChannelCreation;

/**
 * Registers or updates the current registration with an API call. If push notifications are
 * not enabled, this unregisters the device token.
 *
 * Add a `UARegistrationDelegate` to `UAPush` to receive success and failure callbacks.
 */
- (void)updateRegistration;

@end

NS_ASSUME_NONNULL_END
