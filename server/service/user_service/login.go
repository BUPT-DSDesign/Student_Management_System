package user_service

import (
	"errors"
	"server/model/entity/common"
	"server/utils"
)

type loginFlow struct {
	// 包含handler层传来的参数等
	loginForm common.LoginRequest
	userId    int64
}

func newLoginFlow(loginForm common.LoginRequest) *loginFlow {
	return &loginFlow{loginForm: loginForm}
}

func (s *server) DoLogin(loginForm common.LoginRequest) (*common.AccessResponse, error) {
	return newLoginFlow(loginForm).do()
}

func (f *loginFlow) do() (*common.AccessResponse, error) {
	var access *common.AccessResponse

	if err := f.checkNum(); err != nil {
		return nil, err
	}
	if err := f.run(&access); err != nil {
		return nil, err
	}

	return access, nil
}

// 检验参数
func (f *loginFlow) checkNum() error {
	// 检验f.loginForm, 如用户是否存在以及密码是否正确等
	if f.loginForm.Password != "123456" {
		return errors.New("密码错误")
	}
	/*
		调用dao层的CRUD操作
	*/
	f.userId = 123123213213 // 这个赋值在检验参数成功后进行
	return nil
}

func (f *loginFlow) run(access **common.AccessResponse) error {
	// 根据userId得到用户鉴权token
	tokenString, err := utils.ReleaseToken(f.userId)
	if err != nil {
		return err
	}

	*access = &common.AccessResponse{
		UserId: f.userId,
		Token:  tokenString,
	}

	return nil
}
