
#include "net_message.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(sample_test_case, sample_test) {

  enum class CustomMsgTypes : uint32_t { MsgID1, MsgID2 };

  float test_payload_1 = 5.5; // 4 bytes
  bool test_payload_2 = true; // 1 byte
  int test_payload_3 = 3;     // 4 bytes

  // create the message based on the test payload of different types
  custom_netlib::message<CustomMsgTypes> test_msg;
  test_msg.header.id =
      CustomMsgTypes::MsgID1; // my_ids::entry_one;
                              // The header has uint32_t which is 4 bytes and
                              // uint32_t for the size which is another 4 bytes
                              // --> 8 bytes for the header
  test_msg << test_payload_1 << test_payload_2 << test_payload_3;

  // create variables with known wrong entrys and then save their new values
  // based on the message
  float test_returned_payload_1 = 1.1;
  bool test_returned_payload_2 = false;
  int test_returned_payload_3 = 1;

  test_msg >> test_returned_payload_3 >> test_returned_payload_2 >>
      test_returned_payload_1;

  EXPECT_EQ(test_payload_1, test_returned_payload_1);
  EXPECT_EQ(test_payload_2, test_returned_payload_2);
  EXPECT_EQ(test_payload_3, test_returned_payload_3);

/*
  std::cout << "MY TEST:\n"; 
  custom_netlib::message<CustomMsgTypes> test_msg2;
  test_msg2.header.id = CustomMsgTypes::MsgID1;

  uint32_t num_elements = 2;
  uint32_t my_id = 1; 
  uint32_t el1 = 0; 
  uint32_t el2 = 1;
  uint32_t el3 = 2;

  std::vector<uint32_t> input_vec;
  input_vec.push_back(el1); 
  input_vec.push_back(el2);
  input_vec.push_back(el3);
  
   
  for (auto it : input_vec){
    test_msg2 << it; 
  }

  test_msg2 << my_id;
  test_msg2 << num_elements;

  uint32_t counter;
  uint32_t my_new_ID;  
  std::vector<uint32_t> other_ids; 
  uint32_t tmp_el; 

  test_msg2 >> counter; 
  test_msg2 >> my_new_ID; 
  for (int i = 0; i < counter; i++){
    test_msg2 >> tmp_el; 
    other_ids.push_back(tmp_el); 
  }

  std::cout << "The other IDs are:\n";
  for (auto it : other_ids){
    std::cout << it << "\n"; 
  }

  std::cout << "Finished!\n"; 
  */
}