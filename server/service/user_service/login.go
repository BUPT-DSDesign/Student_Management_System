package user_service

import (
	"errors"
	"server/model/dao"
	"server/model/entity/common"
	"server/model/entity/system"
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

	var userInfo *system.UserInfo
	if err := dao.Group.UserDao.QueryUserByName(f.loginForm.Username, &userInfo); err != nil {
		return err
	}
	if userInfo == nil {
		return errors.New("用户不存在, 请先注册！")
	}

	rawPassword, err := utils.Sha1(f.loginForm.Password)
	if err != nil {
		return err
	}

	password, err := utils.Sha256(rawPassword + userInfo.Salt)
	if err != nil {
		return err
	}

	if password != userInfo.Password {
		return errors.New("您的密码错误, 请重新输入！")
	}

	f.userId = userInfo.UserId

	return nil
}

func (f *loginFlow) run(access **common.AccessResponse) error {
	// 根据userId得到用户鉴权token
	tokenString, err := utils.ReleaseToken(f.userId)
	if err != nil {
		return err
	}

	// 根据用户名查验该用户是否是管理员
	isAdmin := false
	if f.loginForm.Username == "admin" {
		isAdmin = true
	}

	*access = &common.AccessResponse{
		UserId:  f.userId,
		Token:   tokenString,
		IsAdmin: isAdmin,
	}

	return nil
}
