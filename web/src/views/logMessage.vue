<template>
    <div class="container">
        <el-descriptions title="日志信息记录" direction="vertical" :column="4" border>
            
        </el-descriptions>
    </div>
</template>

<script>
import { LogStore } from '@/store/log';

export default {
    data() {
        return {
            logs: '' 
        }
    },
    mounted() {
        this.logs = LogStore.logs
    }
}
</script>

<style>
.container {
    overflow-y: scroll;
}
</style>