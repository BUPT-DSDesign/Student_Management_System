<template>
    <div>
        <el-input
        type="text"
        placeholder="编辑个签"
        v-model="signature"
        suffix-icon="el-icon-edit"
        :border="false"
        ref="signature"
        class="signature"
        @focus="edit">
        </el-input>
        <el-dialog title="编辑个签" :visible.sync="dialogVisible" width="30%">
            <el-input placeholder="请输入签名内容" v-model="editSignature" maxlength="30" ref="editSignature" type="text"
            show-word-limit ></el-input>
            <span slot="footer" class="dialog-footer">
                <el-button @click="cancelInput">取 消</el-button>
                <el-button type="primary" @click="confirmInput">确 定</el-button>
            </span>
        </el-dialog>
    </div>
   
</template>

<script>
export default {
    name: 'signature',
    data() {
        return {
            signature: '',
            editSignature: '',
            dialogVisible: false,
        }
    },
    methods: {
        edit() {
            console.log('编辑')
            this.$refs.signature.blur()
            this.dialogVisible = true
        },
        cancelInput() {
            this.editSignature = ''
            this.dialogVisible = false
        },
        confirmInput() {
            this.signature = this.editSignature
            this.dialogVisible = false
        }
    },
}
</script>

<style>
.signature {
    width: 260px;
}

.signature .el-input__inner {
    border: none
}
</style>