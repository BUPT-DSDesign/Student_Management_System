<template>
    <el-upload
        class="avatar-uploader"
        action="http://127.0.0.1:8080/Student_Management_System/user/upload_avatar"
        :headers="headers"
        :show-file-list="false"
        :on-success="handleAvatarSuccess"
        :before-upload="beforeAvatarUpload">
        <img v-if="avatarUrl" :src="avatarUrl" class="avatar" @error="changeToDefault">
        <i v-else class="el-icon-plus avatar-uploader-icon"></i>
    </el-upload>
</template>
<script>
import { Loading } from 'element-ui'
let loadingInstance = null
export default {
    data() {
        return {
            headers: {
                'token': window.localStorage.getItem('token')
            },
            avatarUrl: ''
        };
    },
    mounted() {
        const userId = window.localStorage.getItem('userId')
        this.avatarUrl = `http://127.0.0.1:8080/static/${userId}.jpg`
    },
    methods: {
        changeToDefault() {
            this.avatarUrl = `http://127.0.0.1:8080/static/avatar.jpg`
        },
        handleAvatarSuccess(res, file) {
            loadingInstance = Loading.service({
                lock: true,
                text: '上传头像中，请稍候.....',
                background: 'rgba(0, 0, 0, 0.7)'
            })
            if (res.status_code != 0) {
                loadingInstance.close()
                this.$message({
                    showClose: true,
                    center: true,
                    message: res.status_msg,
                    type: 'error'
                })
                return false
            }
            this.avatarUrl = res.avatar_url

            
            setTimeout(() => {
                loadingInstance.close()
                location.reload()
                this.$message({
                    showClose: true,
                    center: true,
                    message: '更换头像成功',
                    type: 'success'
                })
            }, 500);

        },
        beforeAvatarUpload(file) {
            const isJPG = file.type === 'image/jpeg';
            const isLt2M = file.size / 1024 / 1024 < 2;

            if (!isJPG) {
                this.$message.error('上传头像图片只能是 JPG 格式!');
            }
            if (!isLt2M) {
                this.$message.error('上传头像图片大小不能超过 2MB!');
            }
            return isJPG && isLt2M;
        }
    }
}
</script>

<style>
.avatar-uploader .el-upload {
    border: 1px solid #d9d9d9;
    border-radius: 50%;
    cursor: pointer;
    position: relative;
    overflow: hidden;
}

.avatar-uploader .el-upload:hover {
    border-color: #409EFF;
}

.avatar-uploader-icon {
    font-size: 28px;
    color: #8c939d;
    width: 250px;
    height: 250px;
    line-height: 250px;
    text-align: center;
}

.avatar {
    width: 250px;
    height: 250px;
    display: block;
}
</style>
