/*############################################################################
  # Copyright 2016-2017 Intel Corporation
  #
  # Licensed under the Apache License, Version 2.0 (the "License");
  # you may not use this file except in compliance with the License.
  # You may obtain a copy of the License at
  #
  #     http://www.apache.org/licenses/LICENSE-2.0
  #
  # Unless required by applicable law or agreed to in writing, software
  # distributed under the License is distributed on an "AS IS" BASIS,
  # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  # See the License for the specific language governing permissions and
  # limitations under the License.
  ############################################################################*/

/*!
 * \file
 * \brief EcdsaVerifyBuffer unit tests.
 */

#include <cstdint>
#include <vector>

#include "epid/common-testhelper/epid_gtest-testhelper.h"
#include "gtest/gtest.h"

extern "C" {
#include "epid/common/math/ecdsa.h"
}

namespace {

/// Fill message buffer
/*!

 Fill a message buffer

 \param[in] buf
 pointer to buffer to be filled
 \param[in] buf_len
 size of buffer in bytes

 \returns ::EpidStatus
*/
static EpidStatus fill_message(uint8_t* buf, size_t buf_len) {
  if (!buf) return kEpidBadArgErr;
  if (buf_len <= 0) return kEpidBadArgErr;
  for (size_t n = 0; n < buf_len; n++) {
    buf[n] = (uint8_t)n;
  }
  return kEpidNoErr;
}
class EcdsaVerifyBufferTest : public ::testing::Test {
 public:
  /// an Ecdsa key pair
  /// Regular
  static const EcdsaPrivateKey kPrivkey0;
  static const EcdsaPublicKey kPubkey0;
  /// an Ecdsa key pair
  // Regular
  static const EcdsaPrivateKey kPrivkey1;
  static const EcdsaPublicKey kPubkey1;
  /// a message
  static const std::vector<uint8_t> kMsg0;
  /// a message
  static const std::vector<uint8_t> kMsg1;
  /// signature of msg0 with privkey0
  static const EcdsaSignature kSig_msg0_key0;
  /// signature of msg0 with privkey1
  static const EcdsaSignature kSig_msg0_key1;
  /// signature of msg1 with privkey0
  static const EcdsaSignature kSig_msg1_key0;
  /// signature of msg1 with privkey1
  static const EcdsaSignature kSig_msg1_key1;
  /// signature of empty msg with privkey0
  static const EcdsaSignature kSig_emptymsg_key0;
  /// signature of empty msg with privkey1
  static const EcdsaSignature kSig_emptymsg_key1;
  /// signature of 1M msg with privkey0
  static const EcdsaSignature kSig_1Mmsg_key0;
  /// signature of 1M msg with privkey1
  static const EcdsaSignature kSig_1Mmsg_key1;
};

const EcdsaPrivateKey EcdsaVerifyBufferTest::kPrivkey0 = {
    0xfc, 0x0e, 0x32, 0x3b, 0x5d, 0xf6, 0x95, 0x43, 0x99, 0x0f, 0x46,
    0x37, 0xb0, 0x19, 0xb3, 0xbb, 0xba, 0xd5, 0x91, 0x63, 0x2d, 0x4a,
    0x5a, 0x1e, 0xd8, 0x09, 0x4f, 0x44, 0xa9, 0x5c, 0xd8, 0x87,
};
const EcdsaPublicKey EcdsaVerifyBufferTest::kPubkey0 = {
    0x32, 0x05, 0x28, 0x50, 0x53, 0x0c, 0x62, 0xdf, 0x17, 0xc7, 0xcb,
    0x8c, 0xd3, 0x6f, 0x05, 0xe8, 0xbb, 0xbb, 0x6a, 0xf4, 0x43, 0x82,
    0x59, 0xad, 0xd2, 0x56, 0x6c, 0x63, 0xde, 0x06, 0x37, 0x69, 0x7b,
    0x42, 0x9a, 0xda, 0x3c, 0x91, 0xf3, 0xc0, 0xdd, 0xc1, 0xd4, 0x9e,
    0xc2, 0x24, 0x32, 0x09, 0x07, 0x8c, 0xb9, 0xfc, 0xd8, 0x7b, 0x69,
    0xf9, 0x17, 0x1e, 0x35, 0xb4, 0x20, 0x23, 0x45, 0x94,
};

const EcdsaPrivateKey EcdsaVerifyBufferTest::kPrivkey1 = {
    0xd6, 0x5a, 0x82, 0x5d, 0xd4, 0xad, 0x28, 0x6f, 0x09, 0x2c, 0x96,
    0x07, 0x0a, 0x96, 0xac, 0x13, 0xc9, 0xfd, 0xcf, 0xf0, 0xbe, 0x08,
    0xcb, 0xbb, 0x4b, 0xba, 0x7e, 0x9e, 0xac, 0x02, 0xe9, 0x7c,
};
const EcdsaPublicKey EcdsaVerifyBufferTest::kPubkey1 = {
    0x34, 0xb0, 0x88, 0xcc, 0xb7, 0xf3, 0x56, 0xe0, 0x5c, 0x7c, 0xfc,
    0x03, 0x5d, 0x62, 0x72, 0xe4, 0x70, 0x08, 0xcd, 0x6f, 0x4b, 0x29,
    0x0e, 0x2d, 0x86, 0x26, 0x93, 0x52, 0x55, 0xef, 0xb4, 0x45, 0x11,
    0x07, 0xe9, 0xbe, 0x1e, 0x5d, 0xdb, 0x92, 0x64, 0xb6, 0xbf, 0x67,
    0x83, 0x1f, 0xde, 0xaa, 0x72, 0x84, 0xe9, 0x27, 0x5c, 0xd5, 0xbc,
    0x82, 0x07, 0xc9, 0x81, 0xd1, 0x70, 0x07, 0xaa, 0xd5,
};

const std::vector<uint8_t> EcdsaVerifyBufferTest::kMsg0 = {'m', 's', 'g', '0'};
const std::vector<uint8_t> EcdsaVerifyBufferTest::kMsg1 = {'m', 's', 'g', '1'};

const EcdsaSignature EcdsaVerifyBufferTest::kSig_msg0_key0 = {
    0x81, 0xDD, 0x1A, 0x18, 0x35, 0x1E, 0x22, 0x99, 0x85, 0xC5, 0xAA,
    0xE4, 0x78, 0x02, 0xC5, 0x65, 0xF1, 0x39, 0x80, 0x45, 0xF5, 0xFC,
    0xBD, 0xC6, 0x67, 0xBD, 0xC1, 0x93, 0x42, 0xF6, 0x17, 0x0E, 0x45,
    0x19, 0x39, 0xD3, 0x27, 0x1F, 0xA9, 0x2A, 0x14, 0xDD, 0xF1, 0x5A,
    0x81, 0xA8, 0xEC, 0x3B, 0xB4, 0x0B, 0xF8, 0xC9, 0x9A, 0x06, 0xEB,
    0x04, 0x77, 0x19, 0xCB, 0x57, 0x9C, 0xB9, 0xE9, 0x2F,
};

const EcdsaSignature EcdsaVerifyBufferTest::kSig_msg0_key1 = {
    0xA9, 0xFC, 0x44, 0x2D, 0x09, 0x9E, 0x15, 0x6C, 0x25, 0xD9, 0x74,
    0x9D, 0xD9, 0xAD, 0xC2, 0x7A, 0xFB, 0xC0, 0xD4, 0xFE, 0xC2, 0x48,
    0x41, 0x80, 0x47, 0x4A, 0x74, 0x8A, 0x62, 0x71, 0x68, 0xFF, 0x51,
    0xDF, 0x47, 0x76, 0xAB, 0xD7, 0xE2, 0xA0, 0x97, 0x6B, 0x04, 0x51,
    0x2B, 0x8F, 0x4D, 0x7E, 0xDB, 0x7B, 0x60, 0x08, 0xC6, 0x51, 0x81,
    0x5E, 0x92, 0x9E, 0x11, 0xDB, 0x9C, 0x16, 0x64, 0x79,
};
const EcdsaSignature EcdsaVerifyBufferTest::kSig_msg1_key0 = {
    0xA4, 0xD7, 0xF9, 0x80, 0xBA, 0x93, 0x25, 0xA4, 0x43, 0xD4, 0xCE,
    0x6F, 0x7C, 0xD9, 0xA9, 0xD2, 0x24, 0x1B, 0xB8, 0x32, 0xDE, 0xE0,
    0x26, 0x01, 0xA6, 0xC8, 0x57, 0x21, 0xD7, 0x1E, 0xBC, 0x88, 0xA2,
    0x27, 0x8B, 0x51, 0x93, 0x90, 0xC0, 0x0A, 0x3B, 0xA4, 0x7A, 0x9B,
    0x8C, 0x5E, 0xC8, 0x8B, 0x39, 0x5F, 0x3D, 0x82, 0xCE, 0x57, 0x9F,
    0xF7, 0x9A, 0x32, 0x82, 0xEF, 0xF8, 0xC5, 0x8B, 0x22,
};

const EcdsaSignature EcdsaVerifyBufferTest::kSig_msg1_key1 = {
    0xC1, 0x0A, 0x01, 0xB3, 0x81, 0x47, 0x02, 0xA4, 0xD2, 0xB1, 0xD6,
    0xF9, 0x25, 0x1F, 0xDF, 0x16, 0x7F, 0x28, 0x7F, 0x04, 0x38, 0xAE,
    0x6D, 0x6E, 0xE7, 0xFC, 0xCB, 0x9E, 0xB6, 0xA4, 0xF6, 0x41, 0x2B,
    0xEA, 0x6B, 0x53, 0x32, 0x7B, 0xB0, 0xB6, 0x8F, 0x21, 0x07, 0x0A,
    0x4C, 0xB7, 0xA9, 0xDE, 0x9F, 0x27, 0x3A, 0xEE, 0x4F, 0xE2, 0xF4,
    0xE3, 0x1E, 0xC3, 0x3F, 0xAA, 0x41, 0xD4, 0xFA, 0xA2,
};

const EcdsaSignature EcdsaVerifyBufferTest::kSig_emptymsg_key0 = {
    0xB7, 0xDC, 0x55, 0x35, 0x12, 0xB9, 0xAE, 0x59, 0xAE, 0xE1, 0xB5,
    0x50, 0x20, 0x65, 0x18, 0x84, 0x3B, 0x1E, 0x54, 0xF4, 0x11, 0x4B,
    0x71, 0xBD, 0x12, 0x5E, 0x05, 0x41, 0xAB, 0x6A, 0xF9, 0x20, 0x26,
    0xC9, 0x2B, 0x21, 0x56, 0x62, 0x3F, 0xC3, 0x62, 0xA0, 0xD2, 0x9D,
    0x27, 0x97, 0xF8, 0xE2, 0x59, 0xE7, 0x0E, 0x7E, 0xEE, 0xA7, 0xB5,
    0x11, 0x66, 0xD0, 0xF3, 0x6E, 0x41, 0x76, 0x3C, 0xFE,
};

const EcdsaSignature EcdsaVerifyBufferTest::kSig_emptymsg_key1 = {
    0xCB, 0xED, 0xEF, 0xEE, 0x11, 0x68, 0x21, 0x7D, 0x57, 0x4A, 0x37,
    0xB1, 0x96, 0xF0, 0xF8, 0x42, 0x46, 0x00, 0x64, 0xEF, 0x19, 0xA7,
    0xFF, 0x38, 0x8F, 0x3A, 0x25, 0xD5, 0xC4, 0xF3, 0xD0, 0xC4, 0xB0,
    0xC4, 0xC5, 0xB0, 0x58, 0xAD, 0x74, 0xF2, 0x19, 0xEE, 0x0C, 0x98,
    0x6D, 0x5F, 0x4F, 0xB7, 0x9A, 0xE8, 0x25, 0x1B, 0xAD, 0x9E, 0x26,
    0x22, 0xF2, 0xED, 0xB1, 0x8D, 0x3B, 0x02, 0x7F, 0xB1,
};
const EcdsaSignature EcdsaVerifyBufferTest::kSig_1Mmsg_key0 = {
    0x4A, 0x45, 0xE3, 0xA5, 0xFC, 0x73, 0x86, 0xAF, 0xD8, 0x21, 0x25,
    0x35, 0x13, 0xEB, 0x1B, 0xA7, 0x39, 0x08, 0x21, 0x2A, 0x2A, 0x09,
    0x79, 0x3C, 0xEA, 0x17, 0x67, 0x53, 0x6C, 0xE4, 0xA0, 0x97, 0x36,
    0x98, 0x79, 0x69, 0x1C, 0xF1, 0x21, 0x42, 0x56, 0x48, 0x38, 0x35,
    0x3C, 0xC1, 0x30, 0x7C, 0x49, 0x4F, 0xC9, 0xFA, 0xE1, 0x69, 0xF9,
    0x42, 0x3D, 0x7D, 0x4B, 0x6D, 0xB1, 0xEE, 0x9D, 0x4B,
};

const EcdsaSignature EcdsaVerifyBufferTest::kSig_1Mmsg_key1 = {
    0x87, 0xE0, 0x27, 0x51, 0xC1, 0x16, 0x79, 0x8F, 0xCF, 0x82, 0x60,
    0x48, 0x72, 0x04, 0xE1, 0xC4, 0xDA, 0xEE, 0x31, 0x4D, 0x12, 0xF9,
    0xCA, 0x19, 0x1B, 0xDC, 0xA4, 0xCA, 0x8C, 0xCC, 0xBD, 0x2A, 0xD4,
    0x73, 0x2B, 0x6B, 0xE4, 0x9B, 0xA1, 0x56, 0xF2, 0x28, 0x4A, 0xBD,
    0x41, 0x69, 0x98, 0xAB, 0x62, 0x3D, 0x8D, 0xC9, 0x77, 0xE1, 0x98,
    0xF1, 0x53, 0xF5, 0xD0, 0x97, 0x80, 0x89, 0xCB, 0x97,
};
TEST_F(EcdsaVerifyBufferTest, FailsGivenNullParameters) {
  // tests using a trivial message
  EXPECT_EQ(kEpidBadArgErr,
            EcdsaVerifyBuffer(kMsg0.data(), kMsg0.size(), &kPubkey0, nullptr));
  EXPECT_EQ(kEpidBadArgErr, EcdsaVerifyBuffer(kMsg0.data(), kMsg0.size(),
                                              nullptr, &kSig_msg0_key0));
  // tests using a null message
  EXPECT_EQ(kEpidBadArgErr, EcdsaVerifyBuffer(nullptr, 0, &kPubkey0, nullptr));
  EXPECT_EQ(kEpidBadArgErr,
            EcdsaVerifyBuffer(nullptr, 0, nullptr, &kSig_msg0_key0));
}

TEST_F(EcdsaVerifyBufferTest, VerifiesZeroLengthMsg) {
  // tests using a non-null message buffer
  EXPECT_EQ(kEpidSigValid,
            EcdsaVerifyBuffer(kMsg0.data(), 0, &kPubkey0, &kSig_emptymsg_key0));
  EXPECT_EQ(kEpidSigValid,
            EcdsaVerifyBuffer(kMsg0.data(), 0, &kPubkey1, &kSig_emptymsg_key1));
  // tests using a null message buffer
  EXPECT_EQ(kEpidSigValid,
            EcdsaVerifyBuffer(nullptr, 0, &kPubkey0, &kSig_emptymsg_key0));
  EXPECT_EQ(kEpidSigValid,
            EcdsaVerifyBuffer(nullptr, 0, &kPubkey1, &kSig_emptymsg_key1));
}

TEST_F(EcdsaVerifyBufferTest, VerifiesShortMessage) {
  EXPECT_EQ(kEpidSigValid, EcdsaVerifyBuffer(kMsg0.data(), kMsg0.size(),
                                             &kPubkey0, &kSig_msg0_key0));
  EXPECT_EQ(kEpidSigValid, EcdsaVerifyBuffer(kMsg0.data(), kMsg0.size(),
                                             &kPubkey1, &kSig_msg0_key1));
  EXPECT_EQ(kEpidSigValid, EcdsaVerifyBuffer(kMsg1.data(), kMsg1.size(),
                                             &kPubkey0, &kSig_msg1_key0));
  EXPECT_EQ(kEpidSigValid, EcdsaVerifyBuffer(kMsg1.data(), kMsg1.size(),
                                             &kPubkey1, &kSig_msg1_key1));
}

TEST_F(EcdsaVerifyBufferTest, VerifiesLongMessage) {
  std::vector<uint8_t> msg_1mb(0x100000);
  fill_message(msg_1mb.data(), msg_1mb.size());
  EXPECT_EQ(kEpidSigValid, EcdsaVerifyBuffer(msg_1mb.data(), msg_1mb.size(),
                                             &kPubkey0, &kSig_1Mmsg_key0));
  EXPECT_EQ(kEpidSigValid, EcdsaVerifyBuffer(msg_1mb.data(), msg_1mb.size(),
                                             &kPubkey1, &kSig_1Mmsg_key1));
}

TEST_F(EcdsaVerifyBufferTest, FailsGivenWrongKey) {
  EXPECT_EQ(kEpidSigInvalid, EcdsaVerifyBuffer(kMsg0.data(), kMsg0.size(),
                                               &kPubkey1, &kSig_msg0_key0));
  EXPECT_EQ(kEpidSigInvalid, EcdsaVerifyBuffer(kMsg0.data(), kMsg0.size(),
                                               &kPubkey0, &kSig_msg0_key1));
  EXPECT_EQ(kEpidSigInvalid, EcdsaVerifyBuffer(kMsg1.data(), kMsg1.size(),
                                               &kPubkey1, &kSig_msg1_key0));
  EXPECT_EQ(kEpidSigInvalid, EcdsaVerifyBuffer(kMsg1.data(), kMsg1.size(),
                                               &kPubkey0, &kSig_msg1_key1));
}

TEST_F(EcdsaVerifyBufferTest, FailsGivenWrongMsg) {
  EXPECT_EQ(kEpidSigInvalid, EcdsaVerifyBuffer(kMsg1.data(), kMsg1.size(),
                                               &kPubkey0, &kSig_msg0_key0));
  EXPECT_EQ(kEpidSigInvalid, EcdsaVerifyBuffer(kMsg1.data(), kMsg1.size(),
                                               &kPubkey1, &kSig_msg0_key1));
  EXPECT_EQ(kEpidSigInvalid, EcdsaVerifyBuffer(kMsg0.data(), kMsg0.size(),
                                               &kPubkey0, &kSig_msg1_key0));
  EXPECT_EQ(kEpidSigInvalid, EcdsaVerifyBuffer(kMsg0.data(), kMsg0.size(),
                                               &kPubkey1, &kSig_msg1_key1));
}

TEST_F(EcdsaVerifyBufferTest, FailsGivenInvalidKey) {
  // define a public key with the high half set to all FF. This will
  // not be on the curve specified for this API
  EcdsaPublicKey invalid_pubkey = kPubkey0;
  for (size_t i = 0; i < sizeof(invalid_pubkey.x); i++) {
    invalid_pubkey.x.data[i] = 0xff;
  }
  EXPECT_EQ(kEpidBadArgErr,
            EcdsaVerifyBuffer(kMsg0.data(), kMsg0.size(), &invalid_pubkey,
                              &kSig_msg0_key0));
  EXPECT_EQ(kEpidBadArgErr,
            EcdsaVerifyBuffer(kMsg1.data(), kMsg1.size(), &invalid_pubkey,
                              &kSig_msg1_key0));
}

TEST_F(EcdsaVerifyBufferTest, FailsGivenInvalidSignature) {
  // define a signature with the high half set to all FF. This will
  // not be on the curve specified for this API
  EcdsaSignature invalid_sig = kSig_msg0_key0;
  for (size_t i = 0; i < sizeof(invalid_sig.x); i++) {
    invalid_sig.x.data[i] = 0xff;
  }
  EXPECT_EQ(kEpidBadArgErr, EcdsaVerifyBuffer(kMsg0.data(), kMsg0.size(),
                                              &kPubkey0, &invalid_sig));
}

}  // namespace
