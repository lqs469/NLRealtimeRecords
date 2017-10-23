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

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    _webView.backgroundColor = [UIColor whiteColor];
    _webView.scrollView.scrollEnabled = NO;
    _webView.delegate = self;
    NSString *path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"html"];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:[NSURL URLWithString:path]];
    [_webView loadRequest:request];
    
//    UIBarButtonItem *rightItem = [[UIBarButtonItem alloc] initWithTitle:@"调JS" style:UIBarButtonItemStylePlain target:self action:@selector(rightAction)];
//    self.navigationItem.rightBarButtonItem = rightItem;
}

//- (void)rightAction {
//    NSString *jsStr = [NSString stringWithFormat:@"showAlert('%@')",@"这里是JS中alert弹出的message"];
//    [_webView stringByEvaluatingJavaScriptFromString:jsStr];
//}

#pragma mark - UIWebViewDelegate
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSURL * url = [request URL];
    if ([[url scheme] isEqualToString:@"start"]) {
//        NSArray *params =[url.query componentsSeparatedByString:@"&"];
//
//        NSMutableDictionary *tempDic = [NSMutableDictionary dictionary];
//        for (NSString *paramStr in params) {
//            NSArray *dicArray = [paramStr componentsSeparatedByString:@"="];
//            if (dicArray.count > 1) {
//                NSString *decodeValue = [dicArray[1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
//                [tempDic setObject:decodeValue forKey:dicArray[0]];
//            }
//        }
//        NSLog(@"tempDic:%@",tempDic);
        
        [self onStartAsrButtonClick];
        return NO;
    }
    
    if ([[url scheme] isEqualToString:@"stop"]) {
        [self onStopAsrButtonClick];
    }
    
    if ([[url scheme] isEqualToString:@"copy"]) {
        NSArray *dicArray = [url.query componentsSeparatedByString:@"="];
        if (dicArray.count > 1) {
            NSString *decodeValue = [dicArray[1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
            NSLog(@"result:%@", decodeValue);
            UIPasteboard *appPasteBoard =  [UIPasteboard generalPasteboard];
//            appPasteBoard.persistent = YES;
            NSString *pasteStr = decodeValue;
            [appPasteBoard setString:pasteStr];
            UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"复制成功" message:@"已加入到粘贴板" delegate:self cancelButtonTitle:@"确认" otherButtonTitles:nil];
            [alertView show];
        }
    }
    
    return YES;
}

- (void)cbWithResult:(NSString*)text {
    NSLog(@"+++%@",text);
    NSString *jsStr = [NSString stringWithFormat:@"cb('%@')",text];
    [_webView stringByEvaluatingJavaScriptFromString:jsStr];
}

- (void)onStartAsrButtonClick {
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

- (void)onStopAsrButtonClick {
    //结束语音识别
    [self.recognizer stop];
}

#pragma mark - Notification Callbacks
-(void)asrStatusChanged:(NSNotification*)notify{
    //处理网络变化
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
//            self.asrResultTextView.text = [self.asrResultTextView.text stringByAppendingFormat:@"%@\n",asrOutResult];
            NSLog(@"===%@", asrOutResult);
            [self cbWithResult:asrOutResult];
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
}

-(void) recognizerDidStopRecording:(NlsRecognizer*)recognizer{
    NSLog(@"!!! recognizerDidStopRecording - %@",[NSDate date]);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
