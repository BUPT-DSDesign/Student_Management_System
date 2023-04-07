package user_service

import "server/model/dao"

type editSignatureFlow struct {
	// 包含handler层传来的参数等
	userId    int64
	signature string
}

func newEditSignatureFlow(userId int64, signature string) *editSignatureFlow {
	return &editSignatureFlow{userId: userId, signature: signature}
}

func (s *server) DoEditSignature(userId int64, signature string) error {
	return newEditSignatureFlow(userId, signature).do()
}

func (f *editSignatureFlow) do() error {
	if err := f.checkNum(); err != nil {
		return err
	}
	if err := f.run(); err != nil {
		return err
	}

	return nil
}

// 检验参数
func (f *editSignatureFlow) checkNum() error {
	// 检验f.signature, 如是否出现敏感词汇啥的
	return nil
}

func (f *editSignatureFlow) run() error {
	// 根据f.userId更新数据库, 将其个性签名改成f.signature
	if err := dao.Group.UserDao.UpdateSignature(f.userId, f.signature); err != nil {
		return err
	}
	return nil
}
