package user_service

import (
	"fmt"
	"mime/multipart"
	"server/model/dao"

	"github.com/gin-gonic/gin"
)

type uploadAvatarFlow struct {
	// 包含handler层传来的参数等
	userId int64
	file   *multipart.FileHeader
	c      *gin.Context
}

func newUploadAvatarFlow(userId int64, file *multipart.FileHeader, c *gin.Context) *uploadAvatarFlow {
	return &uploadAvatarFlow{userId: userId, file: file, c: c}
}

func (s *server) DoUploadAvatar(userId int64, file *multipart.FileHeader, c *gin.Context) (*string, error) {
	return newUploadAvatarFlow(userId, file, c).do()
}

func (f *uploadAvatarFlow) do() (*string, error) {
	var avatarUrl *string

	if err := f.checkNum(); err != nil {
		return nil, err
	}
	if err := f.run(&avatarUrl); err != nil {
		return nil, err
	}

	return avatarUrl, nil
}

// 检验参数
func (f *uploadAvatarFlow) checkNum() error {

	// 这里参数一定合法
	return nil
}

func (f *uploadAvatarFlow) run(avatarUrl **string) error {
	// 将图片保存到指定路径
	if err := f.c.SaveUploadedFile(f.file, "./static/"+fmt.Sprintf("%d.jpg", f.userId)); err != nil {
		return err
	}

	// 更新数据库
	savePath := "http://127.0.0.1:8080/static/" + fmt.Sprintf("%d.jpg", f.userId)
	*avatarUrl = &savePath

	// 根据f.userId更新数据库
	dao.UserInfo.AvatarUrl = savePath

	return nil
}
