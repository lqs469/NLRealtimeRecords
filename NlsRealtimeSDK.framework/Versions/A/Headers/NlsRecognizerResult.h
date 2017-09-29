//
//  NlsRecognizerResult.h
//  NlsRealtimeSDK
//
//  Created by Shawn Chain on 13-11-7.
//  Copyright (c) 2015年 Alibaba iDST. All rights reserved.
//

#import <Foundation/Foundation.h>

#define kNlsResponseSuccess @"1";
#define kNlsResponseFail @"0";
#define kNlsResponseVersion10 @"1.0";
#define kNlsResponseVersion40 @"4.0";

#pragma mark - RecognizeResult
/**
 *  语音识别返回的结果，封装了返回值对象
 *
 */
@interface NlsRecognizerResult : NSObject

@property(nonatomic,copy) NSString* version;// 协议版本，目前包括1.0和2.0（数加鉴权的签名字段采用字符串形式）
@property(nonatomic,copy) NSString* request_id;// 当前请求id，若认证未通过则返回0
@property(nonatomic,assign) NSInteger status_code;// 状态码，按照标准http语义实现
@property(nonatomic,assign) NSInteger finish;// 状态码，长语音转写结束时，结果为1，否则为0
@property(nonatomic,copy) NSDictionary* result; // 返回结果，当已有识别结果时返回
@property(nonatomic,copy) NSDictionary* quality; // 返回音频质量，当有最终结果时返回

@end


@interface Result : NSObject

@property(nonatomic,assign) NSInteger sentence_id;// 当前句子序号
@property(nonatomic,assign) NSInteger begin_time;// 当前句子开始时间(ms)
@property(nonatomic,assign) NSInteger end_time;// 当前句子结束时间(ms)，当为streaming模式时，中间结果返回－1
@property(nonatomic,assign) NSInteger status_code;// 状态码，normal模式时，结果为0；streaming模式时，最终结果为0，中间结果为1
@property(nonatomic,copy) NSString* text;// 当前识别结果


@end




