package user_service

import "server/model/entity/common"

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
	return nil
}

func (f *registerFlow) run(**common.AccessResponse) error {
	// 得到token，存入数据库等
	return nil
}
