import { defineStore } from "pinia";
import { LogInfo, LogDelete } from "@/api/log";
import { ref } from "vue";

export const useLogStore = defineStore('log', () => {
    const logs = ref(null)
    const GetLogMessage = async () => {
        try {
            const res = await LogInfo()
            if (res.data.status_code == 0) {
                logs.value = res.data.logs
                return true
            }
            return false
            
        } catch (err) {
            console.log(err)
            return false
        }
    }
    const DeleteLog = async (logId) => {
        try {
            const res = await LogDelete(logId)
            if (res.data.status_code == 0) {
                GetLogMessage()
                return true
            }
            return false
        } catch (err) {
            console.log(err)
            return false
        }
    }
    return {
        logs,
        GetLogMessage,
        DeleteLog
    }
}, {
    persist: true
})