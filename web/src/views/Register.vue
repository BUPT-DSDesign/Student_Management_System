<template>
    <div class="container">
        <div class="register">
            <el-form :model="ruleForm" status-icon :rules="rules" ref="ruleForm" label-width="150px" class="demo-ruleForm">
                 <el-form-item label="学号" prop="studentId">
                    <el-input v-model="ruleForm.studentId"></el-input>
                </el-form-item>
                <el-form-item label="用户名" prop="usename">
                    <el-input v-model="ruleForm.usename"></el-input>
                </el-form-item>
                <el-form-item label="密码" prop="password">
                    <el-input type="password" v-model="ruleForm.password" autocomplete="off"></el-input>
                </el-form-item>
                <el-form-item label="确认密码" prop="checkPass">
                    <el-input type="password" v-model="ruleForm.checkPass" autocomplete="off"></el-input>
                </el-form-item>
                <el-form-item style="text-align: center;">
                    <el-button type="primary" style="margin-right: 60px;" @click="submitForm('ruleForm')">注册</el-button>
                </el-form-item>
            </el-form>
        </div>
    </div>
</template>
<script>
import { UserStore } from '@/store/user'
import { LogStore } from '@/store/log'
import { TimeStore } from '@/store/time'

export default {
    data() {
        var checkStudentId = (rule, value, callback) => {
            if (value.length != 10) {
                return callback(new Error('请输入10位学号'))
            } else {
                callback()
            }
        }
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
                studentId: '',
                password: '',
                checkPass: '',
                usename: ''
            },
            rules: {
                studentId: [
                    {validator: checkStudentId, trigger: 'blur'}
                ],

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
        RegisterIn: async function () {
            // const useUserStore = new useUserStore()
            return await UserStore.RegisterIn(this.ruleForm)
        },
        submitForm(formName) {
            this.$refs[formName].validate(async (valid) => {
                if (valid) {
                    const fg = await this.RegisterIn() // 这里等待请求过程完成
                    if (fg) {                              
                        this.$router.push('/Main/Homepage')
                        this.$message({
                            showClose: true,
                            center: true,
                            message: '注册成功, 直接跳转至主页面',
                            type: 'success'
                        });
                        // 将注册成功的信息写入日志
                        const log = {
                            "create_time":  TimeStore.getTime(),
                            "content": "成功注册账号",
                        }
                        LogStore.addLog(log)
                    }
                } else {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '注册失败',
                        type: 'error'
                    });
                    return false;
                }
            })
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
    width:500px;
    height: 300px;
    margin: auto auto;
    padding:50px;
    border-radius: 10px;
}

</style>
