<template>
    <div class="login-container">
        <el-form :model="ruleForm" :rules="rules" ref="ruleForm" label-width="100px" class="demo-ruleForm">
            <el-form-item label="用户名" prop="name">
                <el-input v-model="ruleForm.name"></el-input>
            </el-form-item>
            <el-form-item label="密码" prop="password">
                <el-input type="password"  v-model="ruleForm.password"></el-input>
            </el-form-item>
            <el-form-item>
                <el-button type="primary" @click="submitForm('ruleForm')">登录</el-button>
            </el-form-item>

        </el-form>
    </div>
</template>

<script>
export default {
    data() {
        return {
            ruleForm: {
                name: '',
                password: '',
            },
            rules: {
                name: [
                    { required: true, message: '请输入用户名', trigger: 'blur' },
                ],
                password: [
                    { required: true, message: '请输入密码', trigger: 'blur' },
                ],

            }
        };
    },
    methods: {
        login: async () => {
            return await userStore.LoginIn(this.ruleForm) 
        },
        submitForm(formName) {
            this.$refs[formName].validate(async (valid) => {
                if (valid) {
                    const flag = await LoginIn()
                    alert('submit!');




                } else {
                    console.log('error submit!!');
                    return false;
                }
            });
        },
    }
}
</script>
<style lang="scss" scoped>
.login-container{
    background-color: #ccc;
    height: 100vh;
    display: flex;
    justify-content: center;
    align-items: center;
}
.el-form{
    width: 300px;
    background-color: #fff;
    padding: 30px;
    border-radius: 10px;
}
.el-form-item{
    margin-top:20px;
}
.el-button{
    width:100%;
    margin-top:20px;
}
</style>

