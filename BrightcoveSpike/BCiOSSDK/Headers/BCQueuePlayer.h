//
//  BCQueuePlayer.h
//  BCiOSSDK
//
//  Created by Jim R. Wilson on 8/1/12.
//  Copyright (c) 2012 Brightcove. All rights reserved.
//

#import "BCMediaPlaybackProtocol.h"
#import "BCQueueManagementProtocol.h"

@protocol BCRenditionSelectionProtocol;

@class BCComponent;
@class BCEventEmitter;
@class BCPlayerItem;
@class BCPlaylist;
@class BCVideo;
@class UIView;

/**
 * The BCQueuePlayer class is extend BCPlayer and manages an AVQueuePlayer.
 */
@interface BCQueuePlayer : NSObject <BCMediaPlaybackProtocol, BCQueueManagementProtocol>

///-----------------------------------------------------------------------------
/// @name Creating the Player
///-----------------------------------------------------------------------------

/**
 * Designated initializer, returns a BCQueuePlayer object initialized with the
 * specified playlist.
 *
 * @param playlistValue A BCPlaylist to stage for playback.
 * @return the initialized object.
 */
- (id)initWithPlaylist:(BCPlaylist *)playlistValue;

/**
 * Convenience initializer, returns a BCQueuePlayer object initialized with the
 * specified BCVideo.
 *
 * @param videoValue a BCVideo to stage for playback.
 * @return the initialized object.
 */
- (id)initWithVideo:(BCVideo *)videoValue;

///-----------------------------------------------------------------------------
/// @name Player Properties
///-----------------------------------------------------------------------------

/**
 * The primary BCEventEmitter used by the BCQueuePlayer.
 *
 * The event emitter communicates all the time-dependent state of the player. If
 * you would like to perform custom logic when an error occurs during playback,
 * for instance, you could add a listener to the emitter for BCEventErrors. All
 * events within the player that are allowed to propagate can be captured and
 * reacted to through this emitter. For a full listing of event types and their
 * structure, refer to the documentation for the player components.
 */
@property (nonatomic, readonly) BCEventEmitter *playbackEmitter;

///-----------------------------------------------------------------------------
/// @name Overrideable Player Components
///-----------------------------------------------------------------------------

/**
 * The sort of component to use for video display.  Subclasses should override
 * this to return a more suitable type when appropriate.
 * @return The component class to use for video display (ex: BCVideoDisplay).
 */
- (Class)displayComponentClass;

/**
 * The actual BCComponent serving as display for this player.  Should be of
 * the type returned by -displayComponentClass.
 * @return The component serving as video display.
 */
@property (nonatomic, readonly) BCComponent *displayComponent;

/**
 * The component used to select renditions for playback.
 */
@property (nonatomic, readonly) id<BCRenditionSelectionProtocol> renditionSelector;

///-----------------------------------------------------------------------------
/// @name Player Presentation
///-----------------------------------------------------------------------------

/**
 * The view containing the movie content.
 *
 * A player is configured by default to resize its subviews when it is resized.
 * You can resize the player view as you would any other UIView:
 *
 * <pre><code>player.view.frame = CGMakeRect(0, 0, 640, 480);</code></pre>
 *
 * For more information on modifying the presentation of a player, see
 * [the View Programming Guide for iOS](http://developer.apple.com/library/ios/#documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Introduction/Introduction.html)
 */
@property (nonatomic, readonly) UIView *view;

/**
 * Customizable view displayed behind the movie content.
 */
@property (nonatomic, readonly) UIView *backgroundView;

///-----------------------------------------------------------------------------
/// @name Other Methods
///-----------------------------------------------------------------------------

/**
 * Factory method to create a BCPlayerItem from a BCVideo object.
 *
 * @param video BCVideo Video to be converted to a BCPlayerItem
 * @return BCPlayerItem representation of the video.
 */
- (BCPlayerItem *)itemForVideo:(BCVideo *)video;

///-----------------------------------------------------------------------------
/// @name Managing AirPlay
///-----------------------------------------------------------------------------

/**
 * Returns whether or not the player allows AirPlay video playback.
 */
- (BOOL)allowsAirPlayVideo;

/**
 * Sets whether or not the player allows AirPlay video playback.
 */
- (void)setAllowsAirPlayVideo:(BOOL)allows;

/**
 * Returns whether or not the player is currently playing video through AirPlay.
 * (read-only)
 */
- (BOOL)airPlayVideoActive;

@end
