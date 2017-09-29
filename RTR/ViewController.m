//
//  ViewController.m
//  RTR
//
//  Created by l on 2017/9/29.
//  Copyright © 2017年 lqs469. All rights reserved.
//

#import "ViewController.h"
#import <NlsRealtimeSDK/NlsRealtimeSDK.h>

@interface ViewController ()<NlsRecognizerDelegate>

@property(nonatomic,strong) NlsRecognizer *recognizer;
@property(nonatomic, strong) UIScrollView *scrollView;
@property(nonatomic, strong) UITextView *asrResultTextView;
@property(nonatomic, strong) UIButton *startAsrButton;
@property(nonatomic, strong) UIButton *stopAsrButton;
@property(nonatomic, strong) UIButton *clearTextButton;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    [self setupUI];
}

- (void)setupUI {
    self.scrollView = [[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height)];
    self.scrollView.showsVerticalScrollIndicator =YES; //垂直方向的滚动指示
    self.scrollView.contentSize = CGSizeMake(self.view.frame.size.width, self.view.frame.size.height - 200);
    [self.view addSubview:self.scrollView];
    
    //Use full screen layout.
    self.edgesForExtendedLayout = UIRectEdgeAll;
    self.automaticallyAdjustsScrollViewInsets = YES;
    self.extendedLayoutIncludesOpaqueBars = YES;
    
    self.asrResultTextView = [[UITextView alloc] initWithFrame:CGRectMake(10, 40, self.view.frame.size.width - 20, 400)];
    self.asrResultTextView.backgroundColor = [UIColor clearColor];//设置它的背景颜色
    self.asrResultTextView.scrollEnabled = YES;//是否可以拖动
    self.asrResultTextView.layer.masksToBounds=YES;
    self.asrResultTextView.layer.borderWidth=1.0;
    self.asrResultTextView.layer.borderColor=[[UIColor blackColor] CGColor];
    [self.scrollView addSubview:self.asrResultTextView];
    
    self.startAsrButton = [[UIButton alloc] initWithFrame:CGRectMake(10, 450, 120, 30)];
    [self.startAsrButton setTitle:@"开始语音识别" forState:UIControlStateNormal];
    self.startAsrButton.backgroundColor = [UIColor blueColor];
    [self.startAsrButton addTarget:self action:@selector(onStartAsrButtonClick:) forControlEvents:UIControlEventTouchUpInside];
    [self.scrollView addSubview:self.startAsrButton];
    
    self.stopAsrButton = [[UIButton alloc] initWithFrame:CGRectMake(140, 450, 120, 30)];
    [self.stopAsrButton setTitle:@"结束语音识别" forState:UIControlStateNormal];
    self.stopAsrButton.backgroundColor = [UIColor blueColor];
    [self.stopAsrButton addTarget:self action:@selector(onStopAsrButtonClick:) forControlEvents:UIControlEventTouchUpInside];
    self.stopAsrButton.enabled = NO;
    self.stopAsrButton.alpha = 0.4;
    [self.scrollView addSubview:self.stopAsrButton];
    
    self.clearTextButton = [[UIButton alloc] initWithFrame:(CGRectMake(270, 450, self.view.frame.size.width - 280, 30))];
    [self.clearTextButton setTitle:@"清除内容" forState:UIControlStateNormal];
    self.clearTextButton.backgroundColor = [UIColor redColor];
    [self.clearTextButton addTarget:self action:@selector(onClearTextButtonClick:) forControlEvents:UIControlEventTouchUpInside];
    self.stopAsrButton.enabled = NO;
    self.stopAsrButton.alpha = 0.4;
    [self.scrollView addSubview:self.clearTextButton];
}

- (void)onStartAsrButtonClick:(id)sender {
    // 初始化ASR请求
    NlsRequest * nlsRequest = [[NlsRequest alloc] init];
    // # warning appkey请从 "快速开始" 帮助页面的appkey列表中获取
    [nlsRequest setAppkey:@"nls-service-shurufa16khz"]; // requested
    // 使用用户自定义热词功能，详见自定义热词文档
    [nlsRequest setVocabularyId:@""];
    // # warning 请修改为您在阿里云申请的数加验证字符串Authorize withSecret,见上方文档
    [nlsRequest Authorize:@"LTAIVcAleKCvMQWe" withSecret:@"Jpo2erfbjmrv99ngN4ZTOClS2Qo7tQ"]; // requested
    // 初始化语音服务核心类
    NlsRecognizer *r = [[NlsRecognizer alloc] initWithNlsRequest:nlsRequest svcURL:nil]; // requested 采用默认svcURL
    r.delegate = self;
    r.cancelOnAppEntersBackground = YES;
    r.enableUserCancelCallback = YES;
    self.recognizer = r;
    NSString *nlsRequestJSONString = [NlsRequest getJSONStringfromNlsRequest:nlsRequest];
    NSLog(@"setupAsrIn : %@",nlsRequestJSONString);
    //开始语音识别
    [self.recognizer start];
}

- (void)onStopAsrButtonClick:(id)sender {
    //结束语音识别
    [self.recognizer stop];
}

#pragma mark - Notification Callbacks
-(void)asrStatusChanged:(NSNotification*)notify{
    //处理网络变化
}

- (void)onClearTextButtonClick:(id)sender {
    self.asrResultTextView.text = @"";
}

#pragma mark - RecognizerDelegate
-(void) recognizer:(NlsRecognizer *)recognizer didCompleteRecognizingWithResult:(NlsRecognizerResult*)result error:(NSError*)error{
    //处理识别结果和错误信息
    //若appkey为流式返回appkey，将会多次回调该方法
    if(error) {
        NSString *errorInfo = [NSString stringWithFormat:@"%@ code= %@\n%@",error.domain,@(error.code),error.userInfo];
        NSLog(@"Error Info: %@", errorInfo);
    } else {
        if (result.result) {
            NSDictionary *getResult = result.result;
            NSString *asrOutResult = [NSString stringWithFormat:@"%@",getResult[@"text"]];
            self.asrResultTextView.text = [self.asrResultTextView.text stringByAppendingFormat:@"%@\n",asrOutResult];
        }
        NSError *jsonError;
        NSString *resultJSONString = [NlsRequest getJSONString:result options:0 error:&jsonError];
        NSLog(@"result: %@", resultJSONString);
    }
}
-(void) recognizer:(NlsRecognizer *)recognizer recordingWithVoiceVolume:(NSUInteger)voiceVolume{
    //处理音量变化
}

-(void) recognizerDidStartRecording:(NlsRecognizer*)recognizer{
    NSLog(@"!!! recognizerDidStartRecording - %@",[NSDate date]);
    NSLog(@"Please speak...");
    
    self.startAsrButton.enabled = NO;
    self.startAsrButton.alpha = 0.4;
    self.stopAsrButton.enabled = YES;
    self.stopAsrButton.alpha = 1;
}

-(void) recognizerDidStopRecording:(NlsRecognizer*)recognizer{
    NSLog(@"!!! recognizerDidStopRecording - %@",[NSDate date]);
    
    self.startAsrButton.enabled = YES;
    self.startAsrButton.alpha = 1;
    self.stopAsrButton.enabled = NO;
    self.stopAsrButton.alpha = 0.4;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
