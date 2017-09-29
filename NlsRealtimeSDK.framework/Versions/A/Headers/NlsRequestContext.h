//
//  NlsRequestContext.h
//  NlsRealtimeSDK
//
//  Created by 刘方 on 12/19/15.
//  Copyright © 2015 Alibaba iDST. All rights reserved.
//

#import <Foundation/Foundation.h>

#pragma mark - HeadersInfo Class Interface
/**
 *  shujia header
 */
@interface HeadersInfo : NSObject

@property (nonatomic, copy) NSString* accept;
@property (nonatomic, copy) NSString* date;
@property (nonatomic, copy) NSString* Authorization;

@end

#pragma mark - AuthBody Class Interface
/**
 *  数加 AuthBody
 */
@interface AuthBody : NSObject

@property (nonatomic, strong) NSMutableArray* requests;

@end

#pragma mark - NlsRequestAuth Class Interface
/**
 *  数加 auth
 */
@interface NlsRequestAuth : NSObject

@property (nonatomic, copy, readonly) NSString* method;
@property (nonatomic, copy, readwrite) NSString* authbBody;
@property (nonatomic, strong) HeadersInfo* headers;

- (NSString *)stringToSign:(NSString *)authId withSecret:(NSString *)authSecret withGMTDate:(NSString *)GMTDate withNlsRequestJSONString:(NSString *)nlsRequestJSONString;

@end

@interface NlsRequestSdkInfo : NSObject
@property (nonatomic, copy, readonly) NSString* version;
@property (nonatomic, copy, readonly) NSString* sdk_type;

@end



#pragma mark - NlsRequestContext Class Interface
/**
 *  NlsRequestContext
 */
@interface NlsRequestContext : NSObject

@property (nonatomic, strong) NlsRequestAuth* auth;
@property (nonatomic, strong) NlsRequestSdkInfo* sdkInfo;// optional

@end
