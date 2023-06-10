package utils

import (
	"errors"
	"github.com/golang-jwt/jwt/v4"
	"time"
)

// token持续时间为
const durationTime = time.Hour

// 加密key
var jwtSecretKey = []byte("DSDesign-group.key")

// Claims 将claims与userId绑定
type Claims struct {
	UserId int64
	jwt.RegisteredClaims
}

// ReleaseToken 颁发token
func ReleaseToken(userId int64) (string, error) {
	expirationTime := time.Now().Add(durationTime) // token过期时间
	claim := &Claims{
		UserId: userId,
		RegisteredClaims: jwt.RegisteredClaims{
			IssuedAt:  jwt.NewNumericDate(time.Now()),
			NotBefore: jwt.NewNumericDate(time.Now()),
			ExpiresAt: jwt.NewNumericDate(expirationTime),
			Issuer:    "DSDesign-group",
		},
	}

	// 得到token
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claim)

	// 以token的字符串的形式返回
	return token.SignedString(jwtSecretKey)
}

// ParseToken 解析token
func ParseToken(tokenString string) (*Claims, error) {
	token, err := jwt.ParseWithClaims(tokenString, &Claims{}, func(token *jwt.Token) (interface{}, error) {
		return jwtSecretKey, nil
	})
	if err != nil {
		return nil, err
	}

	claim, ok := token.Claims.(*Claims)
	if ok && token.Valid {
		return claim, nil
	}

	return nil, errors.New("invalid token")
}
