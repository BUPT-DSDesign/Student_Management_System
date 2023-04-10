<template>
    <div class="edit">
        <el-input
            type="text"
            placeholder="编辑个签"
            v-model="userInfo.signature"
            suffix-icon="el-icon-edit"
            :border="false"
            ref="signature"
            class="signature"
            @focus="edit"
            >
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
import { UserStore } from '@/store/user'
export default {
    name: 'signature',
    props: ['userInfo'],
    data() {
        return {
            editSignature: '',
            dialogVisible: false,
        }
    },
    methods: {
        edit() {
            console.log('编辑')
            console.log(this.userInfo)
            this.$refs.signature.blur()
            this.dialogVisible = true
        },
        cancelInput() {
            this.editSignature = ''
            this.dialogVisible = false
        },
        confirmInput() {
            // 当修改个性签名时, 会发送请求
            const editUserSignature = async () => {
                const fg = await UserStore.EditUserSignature(this.editSignature)
                if (fg) {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '编辑个性签名成功',
                        type: 'success'
                    });
                } else {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '编辑个性签名失败',
                        type: 'error'
                    });
                }
                location.reload()
            }
            editUserSignature()
            this.dialogVisible = false
        }
    },
}
</script>

<style>
.edit {
    /* width: 100%; */
    height: 39px;
    float:left;
}

.signature {
    /* width: 200px; */
}

.signature .el-input__inner {
    border: none;
}
</style>