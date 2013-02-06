//
//  Mi9ViewController.m
//  BrightcoveSpike
//
//  Created by Craig Stanford on 26/11/12.
//  Copyright (c) 2012 ninemsn Pty Ltd. All rights reserved.
//

#import "Mi9ViewController.h"
#import "BCQueuePlayer.h"
#import "BCVideo.h"
#import "BCUIControls.h"
#import "BCEvent.h"
#import "BCCatalog.h"
#import "BCCuePoint.h"
#import <MediaPlayer/MediaPlayer.h>
#import <AdManager/FWSDK.h>

@interface Mi9ViewController ()

@property (nonatomic, strong) BCQueuePlayer* player;
@property (nonatomic, strong) BCUIControls* controls;
@property (nonatomic, strong) id <FWAdManager> adManager;
@property (nonatomic, strong) id <FWContext> adContext;
@property (nonatomic, strong) NSMutableArray* temporalSlots;

@end

@implementation Mi9ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.player = [[BCQueuePlayer alloc] init];
    [self.view addSubview:self.player.view];
    UIView *controlsView = [[UIView alloc] initWithFrame:CGRectMake(0, 180, 320, 50)];
    controlsView.backgroundColor = [UIColor orangeColor];
    [self.view addSubview:controlsView];
    self.controls = [[BCUIControls alloc] initWithEventEmitter:self.player.playbackEmitter andView:controlsView];
    
    [self.player.playbackEmitter on:BCEventWillChangeItem callBlock:^(BCEvent *event) {
        [event preventDefault];
        [self.player.playbackEmitter emit:BCEventPause thenCallBlock:^(BCEvent *event) {
            [self.player seekTo:0];
        }];
    }];
    
//    self.controls.fullScreenButton
    [self submitAdRequest];
    
    // Listen for a cue point to trigger an ad
    [self.player.playbackEmitter on:BCEventCuePoint callBlock:^(BCEvent *event) {
        // Grab the cue point from the event
        BCCuePoint *cuePoint = [event.details objectForKey:@"cuePoints"][0];
        if ([cuePoint.type isEqualToString:@"ad"]) {
            [self.player pause];
            // Here's where you would call your native ad library to play an ad
            // This code will vary a lot depending on what ad library you're using
//            [[[UIAlertView alloc] initWithTitle:@"Advert" message:@"Ads are cool!" delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil] show];
            
            //register callback handler for ad slot playback complete event
            [[NSNotificationCenter defaultCenter] addObserver:self
                                                     selector:@selector(adSlotEnded:)
                                                         name:FW_NOTIFICATION_SLOT_ENDED
                                                       object:self.adContext];
            if (self.temporalSlots.count > 0) {
                id<FWSlot> slot = [self.temporalSlots objectAtIndex:0];
                [self.temporalSlots removeObjectAtIndex:0];
                [slot play];
            } else {
                //if there is no preroll slot, start the content video
                [self.player play];
            }
        }
    }];
    
    BCCuePoint *cuePoint = [BCCuePoint
                            cuePointWithPosition:@"before"
                            type:@"ad"
                            properties:@{ @"adId": @"some-ad-configuration" }];
    [self.player.playbackEmitter emit:BCEventSetCuePoint withDetails:@{ @"cuePoint": cuePoint }];

//    NSString *token = @"Vb3fqavTKFDDZbnnGGtbhKxam7uHduOnob-2MJlpHmUnzSMWbDe5bg.."; //Live read-token
    NSString *token = @"pGhuLmBccsm3_lQTzOBDMvQHikhB6hjUr1IpRfeGd0vK03rhwTmbhA.."; // DEV TOKEN
    NSString *videoId = @"1787715546001";
    BCCatalog *catalog = [[BCCatalog alloc] initWithToken:token];
    [catalog findVideoByID:videoId options:nil callBlock:^(BCError *error, BCVideo *video) {
        [self.player insertVideo:video afterItem:nil];
        [self.player play];
    }];
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)submitAdRequest
{
    self.adManager = newAdManager();
	// contact FreeWheel Integration Support Engineer about configuration of ad network/serverUrl/...
    //set networkId of the customer MRM network
	[self.adManager setNetworkId:183618];  //Ninemsn: 183618
    //set the ad server URL to which all ad requests will go
	[self.adManager setServerUrl:@"http://2cd42c.v.fwmrm.net"];
    
    //Needed for fullscreen take over
    [self.adManager setCurrentViewController:self];
    
    //Array for the slots
    self.temporalSlots = [NSMutableArray array];

    self.adContext = [self.adManager newContext];
    
    //contact FreeWheel Integration Support Engineer about configuration of profiles/slots/keyvalue for your integration.
	[self.adContext setPlayerProfile:@"188283:MSN_AU_iOS_Test"
          defaultTemporalSlotProfile:nil
       defaultVideoPlayerSlotProfile:nil
       defaultSiteSectionSlotProfile:nil];
	
    [self.adContext setSiteSectionId:@"prerollVAST"
                              idType:FW_ID_TYPE_CUSTOM
                      pageViewRandom:0
                           networkId:0
                          fallbackId:0];
    
	[self.adContext setVideoAssetId:@"1787715546001"
                             idType:FW_ID_TYPE_CUSTOM
                           duration:160
                       durationType:FW_VIDEO_ASSET_DURATION_TYPE_EXACT
                           location:nil
                       autoPlayType:true
                    videoPlayRandom:0
                          networkId:0
                         fallbackId:0];
    
    [self.adContext addTemporalSlot:@"preroll" adUnit:@"preroll" timePosition:0 slotProfile:@"" cuePointSequence:0 minDuration:5 maxDuration:60 acceptPrimaryContentType:nil acceptContentType:nil];
    
	//tell the ad context on which base view object to render video ads
    [self.adContext setVideoDisplayBase:self.view];
    
    //regist callback handler for ad request complete event
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(adRequestCompleted:)
                                                 name:FW_NOTIFICATION_REQUEST_COMPLETE
                                               object:self.adContext];
	
	//submit ad request to the Freewheel ad server
	[self.adContext submitRequestWithTimeout:5];
}

- (void)adRequestCompleted:(id)sender
{
    [self.temporalSlots addObjectsFromArray:[self.adContext getSlotsByTimePositionClass:FW_TIME_POSITION_CLASS_PREROLL]];
	for (id<FWSlot> slot in self.temporalSlots) {
		[slot preload];
	}
}

- (void)adSlotEnded:(id)sender
{
    [self.player play];
}

@end
