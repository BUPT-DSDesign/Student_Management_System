<!-- 登录页面 -->
<template>
    <div class="login-container">
        <el-form :model="ruleForm" :rules="rules" ref="ruleForm" label-width="150px" class="demo-ruleForm"
            hide-required-asterisk="false">
            <h1 class="title">用户登陆</h1>
            <el-form-item label="用户名" prop="username">
                <el-input v-model="ruleForm.username" suffix-icon="el-icon-user"
                    @keyup.enter.native="submitForm('ruleForm')"></el-input>
            </el-form-item>
            <el-form-item label="密码" prop="password">
                <el-input type="password" v-model="ruleForm.password" show-password
                    @keyup.enter.native="submitForm('ruleForm')"></el-input>
            </el-form-item>
            <el-link type="primary" @click="jumpReg">注册账号</el-link>
            <el-button type="primary" @click="submitForm('ruleForm')">登录</el-button>
        </el-form>
    </div>
</template>

<script>
import { UserStore } from '@/store/user';
import { LogStore } from '@/store/log';
import { TimeStore } from '@/store/time';

export default {
    data() {
        return {
            ruleForm: {
                username: 'admin',
                password: '123456',
            },
            rules: {
                username: [
                    { required: true, message: '请输入用户名', trigger: 'blur' },
                    { min: 2, max: 10, message: '长度在 2 到 10 个字符', trigger: 'blur' }
                ],
                password: [
                    { required: true, message: '请输入密码', trigger: 'blur' },
                    { min: 6, max: 12, message: '长度在 6 到 12 个字符', trigger: 'blur' }
                ],

            },
        };
    },
    methods: {
        LoginIn: async function () {
            return await UserStore.LoginIn(this.ruleForm)
        },
        submitForm(formName) {
            this.$refs[formName].validate(async (valid) => {
                if (valid) {
                    const fg = await this.LoginIn() // 这里等待请求过程完成
                    if (fg) {
                        if (localStorage.getItem("role") == 'student') {
                            this.$router.push('/studentMain/Homepage')
                            this.$message({
                                showClose: true,
                                center: true,
                                message: '用户登录成功',
                                type: 'success'
                            });
                        }
                        else {
                            this.$router.push('/adminMain/classManage')
                            this.$message({
                                showClose: true,
                                center: true,
                                message: '管理员登录成功',
                                type: 'success'
                            });
                        }
                        // 将登录成功的信息写入日志
                        const log = {
                            "create_time": TimeStore.getTime(),
                            "content": "登录系统",
                        }
                        console.log(log)
                        LogStore.AddLog(log)
                    } else {
                        this.$message({
                            showClose: true,
                            center: true,
                            message: '登录失败, 用户未注册或密码错误',
                            type: 'error'
                        });
                    }
                } else {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '登录失败, 请检查表单',
                        type: 'error'
                    });
                    return false;
                }
            });
        },
        jumpReg() {
            this.$router.push('/Register');
        }
    }
}
</script>
<style lang="scss" scoped>
.login-container {
    background-size: cover;
    background-color: #ccc;
    height: 100vh;
    display: flex;
    justify-content: center;
    align-items: center;
}

.el-form {
    width: 400px;
    background-color: #fff;
    padding: 30px;
    border-radius: 10px;
}

.el-form-item {
    margin-top: 20px;
}

.el-button {
    width: 100%;
    margin-top: 20px;
}

.title {
    margin: 0px auto 40px auto;
    text-align: center;
    color: #66B1FF
}
</style>

