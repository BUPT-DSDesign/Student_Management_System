<!-- 登录页面 -->
<template>
    <div class="login-container">
        <el-form :model="ruleForm" :rules="rules" ref="ruleForm" label-width="100px" class="demo-ruleForm">
             <h1 class="title">用户登陆</h1>
            <el-form-item label="用户名" prop="usename">
                <el-input v-model="ruleForm.usename" suffix-icon="el-icon-user" @keyup.enter.native="submitForm('ruleForm')"></el-input>
            </el-form-item>
            <el-form-item label="密码" prop="password">
                <el-input type="password" v-model="ruleForm.password" show-password @keyup.enter.native="submitForm('ruleForm')"></el-input>
            </el-form-item>
            <el-link type="primary" @click="jumpReg">注册账号</el-link>
            <el-button type="primary" @click="submitForm('ruleForm')">登录</el-button>
        </el-form>
    </div>
</template>

<script>
import { UserStore } from '@/store/user';

export default {
    data() {
        return {
            ruleForm: {
                usename: 'admin',
                password: '123456',
            },
            rules: {
                usename: [
                    { required: true, message: '请输入用户名', trigger: 'blur' },
                    { min: 3, max: 5, message: '长度在 3 到 5 个字符', trigger: 'blur' }
                ],
                password: [
                    { required: true, message: '请输入密码', trigger: 'blur' },
                    { min: 6, max:8, message: '长度在 6 到 8 个字符', trigger: 'blur' }
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
                        if (localStorage.getItem("role") == 'user') {
                            this.$router.push('/stuMain/Homepage')
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
                        
                    }
                   
                } else {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '登录失败',
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

