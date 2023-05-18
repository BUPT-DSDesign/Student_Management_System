package utils

import (
	"crypto/sha1"
	"crypto/sha256"
	"encoding/hex"
	"errors"
	"math/rand"
	"time"
)

// Sha1 SHA1加密
func Sha1(s string) (string, error) {
	hash := sha1.New()
	if _, err := hash.Write([]byte(s)); err != nil {
		return "", err
	}

	return hex.EncodeToString(hash.Sum(nil)), nil
}

// Sha256 SHA256加密
func Sha256(s string) (string, error) {
	hash := sha256.New()
	if _, err := hash.Write([]byte(s)); err != nil {
		return "", err
	}

	return hex.EncodeToString(hash.Sum(nil)), nil
}

// GenerateSalt 盐值生成
func GenerateSalt(size int) (string, error) {
	if size > 10 || size <= 0 {
		return "", errors.New("非法长度,长度应为1-10之间的整数")
	}
	randomNum := []byte{33, 126}
	var result string
	rand.Seed(time.Now().UnixNano())
	for i := 0; i < size; i++ {
		result += string(randomNum)
	}
	return result, nil
}
