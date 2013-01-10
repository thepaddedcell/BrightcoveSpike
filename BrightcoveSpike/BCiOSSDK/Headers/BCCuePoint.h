//
//  BCCuePoint.h
//  BCiOSLibrary
//
//  Created by Keith Peters on 5/17/12.
//  Copyright (c) 2012 Brightcove. All rights reserved.
//

#import "BCComponent.h"

/**
 * Represents the data of a single cue point.
 */
@interface BCCuePoint : NSObject
{
    NSString *position;
    NSString *type;
    NSDictionary *properties;
}

///---------------------------------------------------------------------------------------
/// @name Creating a Cue Point
///---------------------------------------------------------------------------------------

/**
 * Designated initializer, initializes the cue point with a position, type and
 * properties.
 *
 * @param positionValue A string representation of a number or absolute position.
 * @param typeValue A string indicating which type of cue point this is.
 * @param propertiesValue A dictionary of properties describing this cue point.
 * @return Returns the initialized object.
 */
- (id)initWithPosition:(NSString *)positionValue type:(NSString *)typeValue properties:(NSDictionary *)propertiesValue;

/**
 * Factory initializer, initializes the cue point with a position, type and
 * properties.
 *
 * @param positionValue A string representation of a number or absolute position.
 * @param typeValue A string indicating which type of cue point this is.
 * @param propertiesValue A dictionary of properties describing this cue point.
 * @return Returns the initialized object.
 */
+ (id)cuePointWithPosition:(NSString *)positionValue type:(NSString *)typeValue properties:(NSDictionary *)propertiesValue;

/**
 * Initializes the cue point with a position and type.
 *
 * @param positionValue A string representation of a number or absolute position.
 * @param typeValue A string indicating which type of cue point this is.
 * @return Returns the initialized object.
 */
- (id)initWithPosition:(NSString *)positionValue type:(NSString *)typeValue;

/**
 * Convenience factory initializer, initializes the cue point with a position,
 * type and properties.
 *
 * @param positionValue A string representation of a number or absolute position.
 * @param typeValue A string indicating which type of cue point this is.
 * @return Returns the initialized object.
 */
+ (id)cuePointWithPosition:(NSString *)positionValue type:(NSString *)typeValue;

///---------------------------------------------------------------------------------------
/// @name Cue Point Data
///---------------------------------------------------------------------------------------

/**
 * The position at which this cue point will fire.
 * Possible values:
 * - "before"
 * - "after"
 * String representation of an integer in seconds.
 */
@property (nonatomic, readonly) NSString *position;

/**
 * The type of cue point.
 * Possible values include:
 * - "ad"
 * - "bumper"
 */
@property (nonatomic, readonly) NSString *type;

/**
 * Properties of this cue point.
 */
@property (nonatomic, readonly) NSDictionary *properties;

///---------------------------------------------------------------------------------------
/// @name Other Methods
///---------------------------------------------------------------------------------------

/**
 * Method for determining whether one cue point comes before or after another.
 * @param otherCuePoint The other cue point to compare to.
 * @return An NSComparisonResult which represents the outcome of comparison.
 */
- (NSComparisonResult)compare:(BCCuePoint *)otherCuePoint;

@end
