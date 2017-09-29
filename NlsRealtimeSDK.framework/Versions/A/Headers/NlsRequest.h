//
//  NlsRequest.h
//  NlsRealtimeSDK
//
//  Created by 刘方 on 11/17/15.
//  Copyright © 2015 Alibaba iDST. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "NlsRequestContext.h"



#pragma mark - RequestSet Class Interface
@interface RequestSet : NSObject
@property(nonatomic, copy, readonly) NSString *app_key;// 应用appkey
@property (nonatomic, copy, readonly) NSString *format;// 音频数据的格式，目前支持pcm和opu，此处默认opu
@property (nonatomic, assign, readonly) NSInteger sample_rate;//采样率，目前仅支持8000和16000，此处默认为16000
@property (nonatomic, copy, readonly) NSString* response_mode;// 语音识别结果返回模式，返回模式，目前支持normal和streaming，默认为streaming
@property (nonatomic, copy, readonly) NSString* user_id;// 用户id，可选项。使用词表时必选
@property (nonatomic, copy, readonly) NSString* vocabulary_id;// 词表id，可选项。使用词表时必选
@end

#pragma mark - NlsRequest Class Interface
@interface NlsRequest : NSObject

@property(nonatomic,copy, readonly) NSString *version;// 协议版本号
@property(nonatomic,strong) RequestSet *request;
@property(nonatomic,strong) NlsRequestContext *context;


/**
 *  语音请求初始化方法。语音识别、语音合成的语音请求初始化方法。
 *
 *  @param 无
 *
 *  @return self
 */
- (instancetype)init;


/**
 *  设置语音请求的appkey
 *
 *  @param appKey
 *
 *  @return 无
 */
- (void)setAppkey:(NSString *)appKey;

/**
 *  设置语音识别结果返回模式，0是流式，1是非流式。
 *
 *  @param mode
 *
 *  @return 无
 */
- (void)setResponseMode:(NSString *)mode;

/**
 * 设置热词特性的用户id
 *
 *  @param userId
 *
 *  @return 无
 */
- (void)setUserId:(NSString *)userId;

/**
 * 设置热词特性的词表idd
 *
 *  @param vocabularyId
 *
 *  @return 无
 */
- (void)setVocabularyId:(NSString *)vocabularyId;


/**
 *  数加验证，未经过数据验证的语音请求均为非法请求。 其中数加验证所用的时间为Local Date
 *
 *  @param authId 数加验证的ak_id
 *
 *  @param secret 数加验证的ak_secret
 *
 *  @return 无
 */
- (void)Authorize:(NSString *)authId withSecret:(NSString *)secret;

/**
 *  数加验证，未经过数据验证的语音请求均为非法请求。
 *
 *  @param authId 数加验证的ak_id
 *
 *  @param secret 数加验证的ak_secret
 *
 *  @param  GMTDate 用户获取的网络时间，要求GMT格式，如：Wed, 05 Sep. 2012 23:00:00 GMT
 *
 *  @return 无
 */
- (void)Authorize:(NSString *)authId withSecret:(NSString *)secret withGMTDate:(NSString *)GMTDate;

/**
 *  将语音请求NlsRequest转换成JSON字符串形式。NlsRequest --> JSONString。
 *
 *  @param nlsRequest
 *
 *  @return NlsRequest的JSON字符串
 */
+ (NSString *)getJSONStringfromNlsRequest:(NlsRequest *)nlsRequest;

/**
 *  将object转换成JSONString。object --> JSONString。
 *
 *  @param obj
 *  @param options
 *  @param error
 *
 *  @return NlsRequest的JSON字符串
 */
+ (NSString *)getJSONString:(id)obj options:(NSJSONWritingOptions)options error:(NSError**)error;

/**
 *  将object转换成NSDictionary。object --> NSDictionary。
 *
 *  @param obj
 *
 *  @return NSDictionary
 */
+ (NSDictionary *)getObjectData:(id)obj;

@end
