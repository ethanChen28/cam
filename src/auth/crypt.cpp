#include "crypt.h"

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#include <vector>
static std::string key = "l1da4fa54fas4f5Dsda5664aafaAedfL";
static std::string iv = "slf6394jsa5Ds6sf";
int encrypt(const std::string &plainTxt, std::string &encryptTxt) {
  EVP_CIPHER_CTX *ctx;

  /* Create and initialise the context */
  if (!(ctx = EVP_CIPHER_CTX_new())) {
    std::cout << "evp cipher ctx new failed." << std::endl;
    return -1;
  }

  /*
   * Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits
   */
  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                              (const unsigned char *)key.c_str(),
                              (const unsigned char *)iv.c_str())) {
    std::cout << "evp encrypt init failed." << std::endl;
    return -1;
  }

  /*
   * Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  int ciphertext_len;
  std::vector<char> cipherText(plainTxt.size() * 3);
  if (1 != EVP_EncryptUpdate(
               ctx, (unsigned char *)cipherText.data(), &ciphertext_len,
               (const unsigned char *)plainTxt.c_str(), plainTxt.size())) {
    std::cout << "evp encrypt update failed." << std::endl;
    return -1;
  }

  /*
   * Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  int further_len;
  if (1 != EVP_EncryptFinal_ex(
               ctx, (unsigned char *)(cipherText.data() + ciphertext_len),
               &further_len)) {
    std::cout << "evp encrypt final failed." << std::endl;
    return -1;
  }
  ciphertext_len += further_len;
  encryptTxt = std::string(cipherText.data(), ciphertext_len);
  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return 0;
}
int decrypt(const std::string &encryptTxt, std::string &plainTxt) {
  EVP_CIPHER_CTX *ctx;

  /* Create and initialise the context */
  if (!(ctx = EVP_CIPHER_CTX_new())) {
    std::cout << "evp cipher ctx new failed." << std::endl;
    return -1;
  }

  /*
   * Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits
   */
  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                              (const unsigned char *)key.c_str(),
                              (const unsigned char *)iv.c_str())) {
    std::cout << "evp decrypt init failed." << std::endl;
    return -1;
  }

  /*
   * Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary.
   */
  std::vector<char> plainText(encryptTxt.size() * 2);
  int plaintext_len;
  if (1 != EVP_DecryptUpdate(
               ctx, (unsigned char *)plainText.data(), &plaintext_len,
               (const unsigned char *)encryptTxt.c_str(), encryptTxt.size())) {
    std::cout << "evp decrypt update failed." << std::endl;
    return -1;
  }

  /*
   * Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  int further_len;
  if (1 != EVP_DecryptFinal_ex(
               ctx, (unsigned char *)(plainText.data() + plaintext_len),
               &further_len)) {
    std::cout << "evp decrypt final failed." << std::endl;
    return -1;
  }

  plaintext_len += further_len;
  plainTxt = std::string(plainText.data(), plaintext_len);
  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);
  
  return 0;
}