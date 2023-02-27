<template>
    <div class="container">
        <div class="register">
            <el-form :model="ruleForm" status-icon :rules="rules" ref="ruleForm" label-width="100px" class="demo-ruleForm">
                <el-form-item label="用户名" prop="usename">
                    <el-input v-model="ruleForm.usename"></el-input>
                </el-form-item>
                <el-form-item label="密码" prop="password">
                    <el-input type="password" v-model="ruleForm.password" autocomplete="off"></el-input>
                </el-form-item>
                <el-form-item label="确认密码" prop="checkPass">
                    <el-input type="password" v-model="ruleForm.checkPass" autocomplete="off"></el-input>
                </el-form-item>
                <el-form-item>
                    <el-button type="primary" style="margin-right: 60px;" @click="submitForm('ruleForm')">提交</el-button>
                </el-form-item>
            </el-form>
        </div>
    </div>
</template>

<script>
import axios from 'axios';
export default {
    data() {
        var checkName = (rule, value, callback) => {
            if (!value) {
                return callback(new Error('用户名不能为空'));
            }
            else {
                callback();
            }
        };
        var validatePass = (rule, value, callback) => {
            if (value === '') {
                callback(new Error('请输入密码'));
            } else {
                if (this.ruleForm.checkPass !== '') {
                    this.$refs.ruleForm.validateField('checkPass');
                }
                callback();
            }
        };
        var validatePass2 = (rule, value, callback) => {
            if (value === '') {
                callback(new Error('请再次输入密码'));
            } else if (value !== this.ruleForm.password) {
                callback(new Error('两次输入密码不一致!'));
            } else {
                callback();
            }
        };
        return {
            ruleForm: {
                password: '',
                checkPass: '',
                usename: ''
            },
            rules: {
                usename: [
                    { validator: checkName, trigger: 'blur' }
                ],
                pass: [
                    { validator: validatePass, trigger: 'blur' }
                ],
                checkPass: [
                    { validator: validatePass2, trigger: 'blur' }
                ]
            }
        };
    },
    methods: {
        submitForm(formName) {
            let that = this;
            this.$refs[formName].validate((valid) => {
                if (valid) {
                    //post的第一个参数：传入后端的地址，第二个参数：登录数据
                    axios.post("http://localhost:8080/Student_Management_System/user/register", {
                        usename: this.ruleForm.usename,
                        password: this.ruleForm.password
                    }).then
                        (function (response) {
                            let data = response.data;
                            console.log(response);
                            if (data.status_code == 0 && data.status_msg == '注册成功') {
                                that.$router.push('/Main/Homepage');
                            }
                        }, function (err) {
                            console.log(err);
                        })
                } else {
                    console.log('error submit!!');
                    return false;
                }
            });
        }
    }
}
</script>

<style scoped>
.container{
    background-color:#ccc;
    height: 100vh;
     display: flex;
}
.register {
    background-color:#fff;
    width:400px;
    height: 250px;
    margin-top: 100px;
    margin-left:400px;
    padding:50px;
    border-radius: 10px;
}

</style>
