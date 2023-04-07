package user_service

import (
	"errors"
	"server/model/dao"
	"server/model/entity/common"
	"server/model/entity/system"
	"server/utils"
)

type registerFlow struct {
	// 包含handler层传来的参数
	registerForm common.RegisterRequest
	password     string
	salt         string
}

func newRegisterFlow(registerForm common.RegisterRequest, password string, salt string) *registerFlow {
	return &registerFlow{registerForm: registerForm, password: password, salt: salt}
}

func (s *server) DoRegister(registerForm common.RegisterRequest, password string, salt string) (*common.AccessResponse, error) {
	return newRegisterFlow(registerForm, password, salt).do()
}

func (f *registerFlow) do() (*common.AccessResponse, error) {
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
func (f *registerFlow) checkNum() error {
	// 检验f.registerForm, 如用户名是否已经存在等
	var userInfo *system.UserInfo
	if err := dao.Group.UserDao.QueryUserByName(f.registerForm.Username, &userInfo); err != nil {
		return err
	}
	if userInfo != nil {
		return errors.New("用户名已经存在！")
	}
	return nil
}

func (f *registerFlow) run(access **common.AccessResponse) error {
	// 得到userId、token，存入数据库等
	userId, err := utils.GenerateId()
	if err != nil {
		return err
	}

	// 根据得到的userId得到用户鉴权token
	tokenString, err := utils.ReleaseToken(userId)
	if err != nil {
		return err
	}

	// 根据用户名查验该用户是否是管理员
	isAdmin := false
	if f.registerForm.Username == "admin" {
		isAdmin = true
	}

	*access = &common.AccessResponse{
		UserId:  userId,
		Token:   tokenString,
		IsAdmin: isAdmin,
	}

	// 新建一个用户实例
	userInfo := &system.UserInfo{
		Username:  f.registerForm.Username,
		Password:  f.password,
		Salt:      f.salt,
		UserId:    userId,
		StudentId: f.registerForm.StudentId,
		IsAdmin:   isAdmin,
		AvatarUrl: "http://127.0.0.1:8080/static/avatar.jpg",
		Signature: "",
	}

	err = dao.Group.UserDao.AddUser(userInfo)
	if err != nil {
		return err
	}

	return nil
}
