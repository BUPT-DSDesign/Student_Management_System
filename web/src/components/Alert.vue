<template>
    <el-alert title="successMsg" type="success" show-icon center>
    </el-alert>
    <el-alert title="infoMsg" type="info" show-icon center>
    </el-alert>
    <el-alert title="warningMsg" type="warning" show-icon center>
    </el-alert>
    <el-alert title="errorMsg" type="error" show-icon center>
    </el-alert>
</template>

<script>
export default {
    data() {
        return {
            successMsg: '',
            infoMsg: '',
            warningMsg: ''
        } 
    }
}
</script>

