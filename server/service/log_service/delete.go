package log_service

type deleteFlow struct {
	// 包含handler层传来的参数等
	logId int64
}

func newDeleteFlow(logId int64) *deleteFlow {
	return &deleteFlow{logId: logId}
}

func (s *server) DoDelete(logId int64) error {
	return newDeleteFlow(logId).do()
}

func (f *deleteFlow) do() error {
	if err := f.checkNum(); err != nil {
		return err
	}
	if err := f.run(); err != nil {
		return err
	}

	return nil
}

// 检验参数
func (f *deleteFlow) checkNum() error {
	// 这里logId参数一定合法
	return nil
}

func (f *deleteFlow) run() error {
	// 根据logId在数据库删除日志

	return nil
}
