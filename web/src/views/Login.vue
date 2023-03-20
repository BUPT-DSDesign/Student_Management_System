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
            // const useUserStore = new useUserStore()
            return await UserStore.LoginIn(this.ruleForm) 
        },
        submitForm(formName) {
            // let that = this;
            this.$refs[formName].validate(async (valid) => {
                if (valid) {
                    const fg = await this.LoginIn() // 这里等待请求过程完成
                    if (fg) {                              
                        this.$router.push('/Main/Homepage')
                        this.$message({
                            showClose: true,
                            center: true,
                            message: '登录成功',
                            type: 'success'
                        });
                    }
                    // // 下面是之前写的
                    // that.$router.push('/Main/Homepage');
                    // //post的第一个参数：传入后端的地址，第二个参数：登录数据
                    // axios.post("http://localhost:8080/Student_Management_System/user/login", {
                    //     usename: this.ruleForm.usename,
                    //     password: this.ruleForm.password
                    // }).then
                    //     (function (response) {
                    //         let data = response.data;
                    //         console.log(data)
                    //         if (data.status_code == 0 && data.status_msg == '登录成功') {
                    //             that.$router.push('/Main/Homepage');
                    //         }
                    //     }, function (err) {
                    //         console.log(err);
                    //     })
                   
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

