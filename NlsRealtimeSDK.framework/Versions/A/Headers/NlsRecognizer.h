//
//  NlsRecognizer.h
//  NlsRealtimeSDK
//
//  Created by Shawn Chain on 13-10-30.
//  Copyright (c) 2015年 Alibaba iDST. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NlsRequest.h"

/** 
 *  服务状态改变通知
 *  当远端服务状态发生变化的时候，发出该通知。开发者可以根据测返回值，调整UI行为。
 *  object 类型[NSNumber numberWithBool:vailable]，标示主服务是否可用。
 *  userInfo 包含详细的每个服务的状态：{name: [NSNumber numberWithBool:vailable]}。
 */
extern NSString * const kNlsRecognizerServiceStatusChanged;
/**
 *  语音主服务
 */
extern NSString * const kNlsRecognizerServiceNameASR;
/**
 *  SDK版本号1.1.2
 */
extern NSString * const kNlsRecognizerVersion;

/////////////////////////////////////////////////////////////////////////
// Mode definitions
/////////////////////////////////////////////////////////////////////////
/**
 *  SDK模式 （保留方法）
 */
typedef enum{
    kMODE_RECOGNIZER = 0, // 默认模式，用于语音识别
    kMODE_RECORDER = 1, // 录音模式，不进行识别，可回调录音得到的数据
} kNlsRecognizerMode;
/////////////////////////////////////////////////////////////////////////
// Error definitions
/////////////////////////////////////////////////////////////////////////
/**
 *  语音识别错误代码
 */
typedef enum{
    kERR_NO_ERROR = 0,
    kERR_GENERIC_ERROR = 1,
    kERR_USER_CANCELED = 520, // 用户取消
    kERR_NETWORK_ERROR = 530, // 网络及通讯异常
    kERR_SERVICE_ERROR = 540, // 语音服务异常或被降级
    kERR_VOICE_ERROR = 550,   // 录音及语音识别异常，或用户未说话
    kERR_MIC_ERROR = 560,     // Mic无法访问或硬件异常
    kERR_TOOSHORT_ERROR = 570,  // 用户点击过快
} kNlsRecognizerErrorCode;

/////////////////////////////////////////////////////////////////////////
#pragma mark - NlsRecognizer Delegate
/////////////////////////////////////////////////////////////////////////
@class NlsRecognizer;
@class NlsRecognizerResult;

/**
 *  NlsRecognizer的回调接口
 */
@protocol NlsRecognizerDelegate <NSObject>

@required
/**
 *  语音识别的关键回调函数，delegate必须实现。
 *
 *  @param recognizer NlsRecognizer
 *  @param result 返回值对象 NlsRecognizerResult
 *  @param error  语音识别错误和异常 NSError
 *
 *  @return 无
 */
-(void)recognizer:(NlsRecognizer *)recognizer didCompleteRecognizingWithResult:(NlsRecognizerResult*)result error:(NSError*)error;

@optional

/**
 *  语音识别模式下，返回语音数据，调用频率取决于SDK内部设定。
 *  @param recognizer NlsRecognizer
 *  @param frame 返回的语音
 *
 *  @return 无
 */
-(void)recognizer:(NlsRecognizer *)recognizer recordingWithVoiceData:(NSData*)voiceData;

/**
 *  语音识别模式下，返回语音音量，调用频率取决于SDK内部设定。
 *  @param recognizer NlsRecognizer
 *  @param voiceVolume 0-100的数值
 *
 *  @return 无
 */
-(void)recognizer:(NlsRecognizer *)recognizer recordingWithVoiceVolume:(NSUInteger)voiceVolume;

/**
 *  语音识别模式下，开始语音识别的回调通知
 *
 *  @param NlsRecognizer
 *
 *  @return 无
 */
-(void)recognizerDidStartRecording:(NlsRecognizer *)recognizer;

/**
 *  语音识别模式下，停止语音识别的回调通知
 *
 *  @param NlsRecognizer
 *
 *  @return 无
 */
-(void)recognizerDidStopRecording:(NlsRecognizer *)recognizer;

/**
 *  录音模式下，停止的回调通知，可得到录音数据
 *  
 *  @param NlsRecognizer
 *
 *  @return 无
 */
-(void)recognizerDidStopRecording:(NlsRecognizer *)recognizer withRecorderData:(NSData*)data;


@end


/////////////////////////////////////////////////////////////////////////
#pragma mark - NlsRecognizer Interface
/////////////////////////////////////////////////////////////////////////

/**
 *  NlsRecognizer
 *
 *  语音识别SDK的核心类，封装了录音设备的初始化，压缩处理，语音检测（VAD）等复杂逻辑，自动的将语音数据同步传送到语音服务器上。开发者只需要传递正确的delegate，就能完成语音识别合成
 *
 */
@interface NlsRecognizer : NSObject

/**
 *  NlsRecognizer delegate
 *  语音识别结果返回或者发生异常的回调接口
 */
@property(nonatomic,assign) id<NlsRecognizerDelegate> delegate;

/**
 *  缺省为NO. 如果设为YES，则SDK会监听App状态，一旦切换到后台，就自动取消请求。
 */
@property(nonatomic,assign,readwrite) BOOL cancelOnAppEntersBackground;

/**
 *  NlsRecognizer的cancel方法被调用的时候，会触发delegate的onRecognizeComplete方法，错误码为kERR_USER_CANCELED,错误信息为kNlsRecognizerErrorUserCanceled。如果App不想被回调，请设置为NO即可。参见 cancel方法。
 */
@property(nonatomic,assign,readwrite) BOOL enableUserCancelCallback;

/**
 *  SDK工作模式设置
 *  语音SDK的工作模式，默认为kMODE_RECOGNIZER，打开kMODE_RECORDER模式时，停止录音时回调recognizerDidStopRecording:withRecorderData:
 */
@property(nonatomic,assign,readwrite) kNlsRecognizerMode mode;

/**
 *  打开语音记录功能，SDK将会把语音识别的文件记录到当前App的document下面，调试用。缺省关闭，在DEBUG模式下有效，Release模式下无效。
 */
@property(nonatomic,assign,readwrite) BOOL enableVoiceLog;

/**
 *  enable or unable for websocket, default true
 */
@property(nonatomic,assign,readwrite) BOOL enableSSL;



/**
 *  初始化NlsRecognizer
 *  注意: 通过该API使用的appKey必须提前通过configure函数预先配置好
 *
 *  @param nlsRequest
 *  @param svcURL
 *
 *  @return 无
 */
-(id)initWithNlsRequest:(NlsRequest *)nlsRequest svcURL:(NSString*)svcURL;


/**
 * enable set AudioSessionSetActive(NO),default is YES; set NO will not
 */
-(void)setAudioSessionActiveApiEnable:(BOOL) enable;



/**
 *  开始语音识别。
 *  打开录音设备，同时开始识别。
 *
 *  @param 语音服务地址
 *
 *  @return 无
 */
-(BOOL)start;

/**
 *  停止语音识别
 *  停止录音设备，delegate的didStopRecord会被回调。网络请求在后台继续，如果有识别结果返回，则会通过delegate的didCompleteRecognizingWithResult回调方法单独返回。
 *
 *  @param 无
 *
 *  @return 无
 */
-(void)stop;

/**
 *  取消语音识别
 *  取消语音识别，录音会停止，网络请求会取消。根据enableUserCancelCallback的设置，决定是否回掉delegate方法。
 *
 *  @param 无
 *
 *  @return 无
 */
-(void)cancel;

/**
 *  语音识别是否已经开始
 *
 *  @param 无
 *
 *  @return 语音识别是否已经开始
 */
-(BOOL)isStarted;

/**
 *  用于自然语言处理解NLU、语音合成TTS的启动方法
 *  网络请求在后台继续，如果有识别结果返回，则会通过delegate的didCompleteRecognizingWithResult回调方法返回文本结果，通过didCompleteTTSWithVoiceData回调方法返回语音数据。
 *
 *  @param 无
 *
 *  @return 无
 */
-(void)sendText;

@end
